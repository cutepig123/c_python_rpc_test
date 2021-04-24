// file:///N:/3rd_sw/boost/boost_1_54_0/doc/html/interprocess/quick_guide.html#interprocess.quick_guide.qg_memory_pool
//

#include <boost/interprocess/ipc/message_queue.hpp>
#include <iostream>
#include <vector>
#include <windows.h>

using namespace boost::interprocess;

int p1 ()
{
   try{
	   printf("open\n");
      message_queue mq
		  (open_only        //only create
		  ,"message_queue"  //name
		  );

	  printf("send\n");
      //Send 100 numbers
      for(int i = 0; i < 100; ++i){
         mq.send(&i, sizeof(i), 0);
      }
	  printf("done\n");
   }
   catch(interprocess_exception &ex){
      std::cout << ex.what() << std::endl;
      return 1;
   }

   return 0;
}

int p2 ()
{
   try{
      printf("create\n");
      message_queue::remove("message_queue");
	  message_queue mq
		  (create_only               //only create
		  ,"message_queue"           //name
		  ,100                       //max message number
		  ,sizeof(int)               //max message size
		  );

      unsigned int priority;
      message_queue::size_type recvd_size;
	  printf("receive\n");
      //Receive 100 numbers
      for(int i = 0; i < 100; ++i){
         int number;
         mq.receive(&number, sizeof(number), recvd_size, priority);
         if(number != i || recvd_size != sizeof(number))
            return 1;
      }
	  printf("done\n");
   }
   catch(interprocess_exception &ex){
      message_queue::remove("message_queue");
      std::cout << ex.what() << std::endl;
      return 1;
   }
   message_queue::remove("message_queue");
   return 0;
}

int main(int argc, char *argv[])
{
	for (int i=0; i<argc; i++)
	{
		printf(">>arg[%d]=%s\n", i, argv[i]);
	}

	if(argc==1)
	{
		char s[_MAX_PATH];
		sprintf(s, "start %s p2", argv[0]);
		system(s);
		

		system("pause");
		p1();
	}
	else
	{
		p2();
	}
	system("pause");
	return 0;
}
