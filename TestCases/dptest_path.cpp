#include "core/path.h"
#include "core/file_system.h"
#include "platform/platform.h"
#include "utils/unit_test.h"
using namespace sims;

UNIT_TEST(Path)
{
	// Join
	{
		string doc1 = "/doc";
		string doc2 = "/doc/";
		string a1 = "a.txt";
		string a2 = "/a.txt";

		string expect = "/doc/a.txt";
		EXPECT_EQ(Path::Join(doc1, a1), expect);
		EXPECT_EQ(Path::Join(doc1, a2), expect);
		EXPECT_EQ(Path::Join(doc2, a1), expect);
		EXPECT_EQ(Path::Join(doc2, a2), expect);
	}

	// Parent
	{
		string path1 = "/doc/a.txt";
		string path2 = "/doc/";
		string path3 = "/doc";
		EXPECT_EQ(Path::Parent(path1), string("/doc"));
		EXPECT_EQ(Path::Parent(path2), string("/"));
		EXPECT_EQ(Path::Parent(path3), string("/"));
	}

	// Filename
	{
		string path1 = "/doc/a.txt";
		string path2 = "/doc";
		string path3 = "/";
		EXPECT_EQ(Path::Filename(path1), string("a.txt"));
		EXPECT_EQ(Path::Filename(path2), string("doc"));
		EXPECT_EQ(Path::Filename(path3), string(""));
	}

	// Extension
	{
		string path1 = "/doc/a.txt";
		string path2 = "/doc/a";
		string path3 = "/doc/a.";
		EXPECT_EQ(Path::Extension(path1), string(".txt"));
		EXPECT_EQ(Path::Extension(path2), string(""));
		EXPECT_EQ(Path::Extension(path3), string("."));
	}

	// Normalize
	{
		string path1 = "/doc/../a.txt";
		string path2 = "/doc/../ad/a.txt";
		string path3 = "/doc/ad/././a.txt";
		EXPECT_EQ(Path::Normalize(path1), string("/a.txt"));
		EXPECT_EQ(Path::Normalize(path2), string("/ad/a.txt"));
		EXPECT_EQ(Path::Normalize(path3), string("/doc/ad/a.txt"));
	}
}