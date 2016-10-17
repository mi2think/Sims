/********************************************************************
	created:	2016/10/17
	created:	17:10:2016   11:38
	filename: 	D:\Code\Sims\Sims\core\vfs.h
	file path:	D:\Code\Sims\Sims\core
	file base:	vfs
	file ext:	h
	author:		mi2think@gmail.com
	
	purpose:	Virtual File System
*********************************************************************/
#ifndef __VFS_H__
#define __VFS_H__

#include "sims.h"
#include "file_system.h"
#include "file_input_stream.h"
#include "file_output_stream.h"

namespace sims
{
	/** VFS is used for simplify path specifying also support:
	        1. support search path by the order of mount
	        2. using relative path when access file, making code more portable

		use case:
		    mount "/res/models/texture" as "texture", then you can use
		    "texture/a.png" as "/res/models/texture/a.png"
		here we mark:
		    "texture/a.png"        : VFS path         (vfsPath)
			"texture"              : Mount Path       (mountPath)
			"/res/models/texture/" : FileSystem Path  (fsPath)
	 */

	class VFS : public IFileSystem, public NonCopyable
	{
	public:
		static VFS& GetVFS();

		static void Init();
		static void Shutdown();

		VFS();
		~VFS();

		// mount file system so that the contents of 'fsPath' appears at 'mountPath'
		void Mount(const IFileSystemRef& fs, const string& fsPath, const string& mountPath);

		// unmount a path
		void Unmount(const string& mountPath);

		// clear all paths
		void Clear();

		virtual IInputStreamRef OpenInputStream(const string& path);
		virtual IOutputStreamRef OpenOutputStream(const string& path);
		virtual bool GetFileInfo(const string& path, FileInfo& fi) const;
		virtual bool DeleteFile(const string& path);
	private:
		struct MountNode
		{
			MountNode(const IFileSystemRef& _fs, const string& _fsPath, const string& _mountPath)
				: fs(_fs)
				, fsPath(_fsPath)
				, mountPath(_mountPath)
			{}

			IFileSystemRef fs;
			string fsPath;
			string mountPath;
		};

		// return true if given mount is a match for a normalized VFS path
		bool IsMount(const MountNode* mount, const string& vfsPath) const;

		// transform a normalized VFS path into a mount and fsPath, return the first mount if exists
		bool LookupInput(const string& vfsPath, MountNode*& mountNode, string& fsPath) const;

		// transform a normalized VFS path into a mount and fsPath, return the first mount if exists
		bool LookupOutput(const string& vfsPath, MountNode*& mountNode, string& fsPath) const;

		// transform a normalized VFS path into the FS path for a given mount
		string GetFSPath(const MountNode* mount, const string& vfsPath) const;

		vector<MountNode*> mounts_;
	};
}

#endif
