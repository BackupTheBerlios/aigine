#include "Model/AigineThread.h"
#include "Model/Liste.h"
#include <string>
#define THREAD_COUNT 10

using namespace std;

int main()

{
	int i = 0;
	Liste<AigineThread*> *threadList = new Liste<AigineThread*>;
	for(i = 1; i <= THREAD_COUNT; i++)
	{
		threadList->addItem(new AigineThread("thread"));
	}	
	for(i = 1; i <= THREAD_COUNT; i++)
	{
		AigineThread* tmpThread = *threadList->getData(i);
		tmpThread->Start();
	}
	return 0;
}