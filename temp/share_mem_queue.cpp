#include <windows.h>
#include "event.h"
#include "lock.h"
#include <boost/ptr_container/ptr_vector.hpp>

#define QUEUE_MAGIC_NUM	0x123456

/*
Q: typical usage?
client: malloc() --> send the memory identifier to server --> wait rely --> free

client_malloc:
	for each block:
		if *block.isIdle.lock():
			*block.isIdle.lock()=false
			return block
	return null

client_free(block):
	b

Q: ownership?
a block's malloc/free can only be managed by client
a block can only be owned by one client at same time

Q: How to do synchronization?
every memory block has 
a isIdle flag: critical<bool> true = the block is idle, no one ownes it. false = the block is active, one client owns it

Q: Does it support multiple clients?
Q: Does it support multiple servers?

*/
class ShareMemoryQueue
{
	HANDLE m_hMapFile;
	LPVOID m_pBuf;

	test::Mutex m_mtxQueue;

	boost::ptr_vector<BlockMgr>	m_blocks;

	struct Header
	{
		size_t magic_num;
		size_t nblock;
		size_t blockSize;
	};

	struct Block{
		char isIdle;
		char data[1];
	};

	static std::string concat(const char *s, int index)
	{
		char t[100];
		sprintf(t, "%s-%d", s, index);
		return t;
	}

	struct BlockMgr
	{
		int index;
		test::Mutex mtx;
		GraphApi::Event evtMsgReady;
		GraphApi::Event evtRpyReady;

		explicit BlockMgr(int i)
			:index(i)
			, mtx(concat("blockmutex", i))
			, evtMsgReady(false, concat("evtMsgReady", i))
			, evtRpyReady(false, concat("evtRpyReady", i))
		{
		}
	};

	static size_t calc_block_size(size_t dataSize)
	{
		return sizeof(Block) + dataSize;
	};

	static size_t calc_queue_size(size_t nblock, size_t dataSize)
	{
		size_t t = sizeof(Header);
		
		t += calc_block_size(dataSize)*nblock;
		return t;
	}

	Header& header()
	{
		return *(Header*)m_pBuf;
	}

	size_t& magic_num()
	{
		return header().magic_num;
	}

	size_t& nblock()
	{
		return header().nblock;
	}
	size_t& blockSize()
	{
		return header().blockSize;
	}

	Block& block(size_t index)
	{
		assert(index < nblock());
		return *(Block*)(m_pBuf + sizeof(Header) + blockSize()*index);
	}
	
public:
	// queue structure
	/*struct Queue{
		size_t magic_num;
		size_t nblock;
		size_t blockSize;
		struct block
		{
			char isIdle;	// idle or not
			data
		};

		block blocks[N];
	};*/

	ShareMemoryQueue(size_t nblock, size_t dataSize):m_mtxQueue("MyQueueMutexObject")
	{
		HANDLE hMapFile;
		bool isCreator = false;
		test::Lock lock(m_mtxQueue);
		
		const char *szName = "Global\\MyFileMappingObject";
		
		hMapFile = OpenFileMapping(
			FILE_MAP_ALL_ACCESS,   // read/write access
			FALSE,                 // do not inherit the name
			szName);               // name of mapping object

		if (hMapFile == NULL)
		{
			size_t queueSize = calc_queue_size(nblock, dataSize);
			hMapFile = CreateFileMapping(
				INVALID_HANDLE_VALUE,    // use paging file
				NULL,                    // default security
				PAGE_READWRITE,          // read/write access
				0,                       // maximum object size (high-order DWORD)
				queueSize,                // maximum object size (low-order DWORD)
				szName);                 // name of mapping object
			isCreator = true;
		}

		if (hMapFile == NULL)
			throw std::exception("cannot get share memory handle");
		
		pBuf = (LPTSTR) MapViewOfFile(hMapFile, // handle to map object
			FILE_MAP_ALL_ACCESS,  // read/write permission
			0,
			0,
			BUF_SIZE);

		if (pBuf == NULL)
			throw std::exception("cannot get share memory address");

		m_hMapFile = hMapFile;
		m_pBuf = pBuf;

		if (isCreator)
		{
			magic_num() = QUEUE_MAGIC_NUM;
			nblock() = nblock();
			blockSize() = calc_block_size(dataSize);
		}
		else
		{
			assert(magic_num() == QUEUE_MAGIC_NUM);
			assert(nblock() == nblock());
			assert(blockSize() == calc_block_size(dataSize));
		}

		for (size_t i=0; i<nblock(); i++)
		{
			m_blocks.push_back(new BlockMgr(i));
		}
	}

	~ShareMemoryQueue()
	{
		UnmapViewOfFile(m_pBuf);

		CloseHandle(m_hMapFile);
	}

	/*******
	Find a block with idle=true
	*****************************************************************/
	void* getblockAndLock()
	{

	}
	void unlockblock(void *)
	{

	}
};

