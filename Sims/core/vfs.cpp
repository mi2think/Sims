/********************************************************************
	created:	2016/10/17
	created:	17:10:2016   14:19
	filename: 	D:\Code\Sims\Sims\core\vfs.cpp
	file path:	D:\Code\Sims\Sims\core
	file base:	vfs
	file ext:	cpp
	author:		mi2think@gmail.com
	
	purpose:	Virtual File System
*********************************************************************/
#include "vfs.h"
#include "path.h"
#include "platform/platform.h"
#include "utils/string_utils.h"

namespace sims
{
	static Ref<VFS> s_VFS;

	VFS& VFS::GetVFS()
	{
		return *s_VFS;
	}

	void VFS::Init()
	{
		s_VFS = new VFS();
	}

	void VFS::Shutdown()
	{
		s_VFS = nullptr;
	}

	VFS::VFS()
	{}

	VFS::~VFS()
	{
		Clear();
	}

	void VFS::Mount(const IFileSystemRef& fs, const string& fsPath, const string& mountPath)
	{
		string mountPathN = Path::Normalize(mountPath);
		if (mountPathN.empty())
			mountPathN = "/";

		MountNode* mount = new MountNode(fs, fsPath, mountPathN);
		mounts_.insert(mounts_.begin(), mount); // last insert has top priority
	}

	void VFS::Unmount(const string& mountPath)
	{
		string mountPathN = Path::Normalize(mountPath);
		for (auto it = mounts_.begin(); it != mounts_.end();)
		{
			MountNode* mount = *it;
			if (mount->mountPath == mountPath)
			{
				it = mounts_.erase(it);
				delete mount;
			}
			else
				++it;
		}
	}

	void VFS::Clear()
	{
		for (auto mount : mounts_)
		{
			delete mount;
		}
		mounts_.clear();
	}

	bool VFS::IsMount(const MountNode* mount, const string& vfsPath) const
	{
		if (!str_starts_with(vfsPath, mount->mountPath))
			return false;

		return mount->mountPath.back() == '/' ||
			vfsPath.size() == mount->mountPath.size() ||
			vfsPath[mount->mountPath.size()] == '/';
	}

	bool VFS::LookupInput(const string& vfsPath, MountNode*& mountNode, string& fsPath) const
	{
		for (auto& mount : mounts_)
		{
			if (IsMount(mount, vfsPath))
			{
				// possible match, transform into FS path and check for file/direction exists
				fsPath = GetFSPath(mount, vfsPath);
				if (mount->fs->Exists(fsPath))
				{
					mountNode = mount;
					return true;
				}
			}
		}
		return false;
	}

	bool VFS::LookupOutput(const string& vfsPath, MountNode*& mountNode, string& fsPath) const
	{
		for (auto& mount : mounts_)
		{
			if (IsMount(mount, vfsPath))
			{
				fsPath = GetFSPath(mount, vfsPath);
				mountNode = mount;
				return true;
			}
		}
		return false;
	}

	string VFS::GetFSPath(const MountNode* mount, const string& vfsPath) const
	{
		if (vfsPath == mount->mountPath)
			return mount->fsPath;

		string filePath = vfsPath.substr(mount->mountPath.size(), vfsPath.size() - mount->mountPath.size());
		return Path::Join(mount->fsPath, filePath);
	}

	IInputStreamRef VFS::OpenInputStream(const string& path)
	{
		string vfsPath = Path::Normalize(path);
		MountNode* mount;
		string fsPath;
		if (LookupInput(vfsPath, mount, fsPath))
		{
			return mount->fs->OpenInputStream(fsPath);
		}
		return Platform::GetFileSystem()->OpenInputStream(fsPath);
	}

	IOutputStreamRef VFS::OpenOutputStream(const string& path)
	{
		string vfsPath = Path::Normalize(path);
		MountNode* mount;
		string fsPath;
		if (LookupOutput(vfsPath, mount, fsPath))
		{
			return mount->fs->OpenOutputStream(fsPath);
		}
		return Platform::GetFileSystem()->OpenOutputStream(fsPath);
	}

	bool VFS::GetFileInfo(const string& path, FileInfo& fi) const
	{
		string vfsPath = Path::Normalize(path);
		MountNode* mount;
		string fsPath;
		if (LookupInput(vfsPath, mount, fsPath))
		{
			return mount->fs->GetFileInfo(fsPath, fi);
		}
		return Platform::GetFileSystem()->GetFileInfo(path, fi);
	}

	bool VFS::DeleteFile(const string& path)
	{
		string vfsPath = Path::Normalize(path);
		MountNode* mount;
		string fsPath;
		if (LookupInput(vfsPath, mount, fsPath))
		{
			return mount->fs->DeleteFile(fsPath);
		}
		return Platform::GetFileSystem()->DeleteFile(path);
	}
}