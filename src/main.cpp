#include <string>
#include <thread>

// TODO make these imports more explicit
#include "filebuffer.h"
#include "messages.h"

using namespace std;

void SendMessagesFromWorker1()
{
  FileBuffer buffer;

  for (auto i = 0; i < 10 * 1000; ++i)
  {
    TraderKeyLoginMessage traderKeyLoginMessage;
    traderKeyLoginMessage.SetTraderName("Trader1");
    traderKeyLoginMessage.SetTraderIndex(1);
    traderKeyLoginMessage.SetFooFactor(12.57);
    buffer.Serialise(traderKeyLoginMessage);

    OrderInsertMessage orderInsertMessage;
    orderInsertMessage.SetVolume(100);
    orderInsertMessage.SetPrice(29.4);
    buffer.Serialise(orderInsertMessage);
  }
}

void SendMessagesFromWorker2()
{
  FileBuffer buffer;

  for (auto i = 0; i < 15 * 1000; ++i) // will just be under the 1Mb buffer size
  {
    TraderKeyLoginMessage traderKeyLoginMessage;
    traderKeyLoginMessage.SetTraderName("Trader2");
    traderKeyLoginMessage.SetTraderIndex(2);
    traderKeyLoginMessage.SetFooFactor(8.27);
    buffer.Serialise(traderKeyLoginMessage);

    OrderInsertMessage orderInsertMessage;
    orderInsertMessage.SetVolume(31);
    orderInsertMessage.SetPrice(4.51);
    buffer.Serialise(orderInsertMessage);
  }
}

int main()
{
  std::thread worker1(SendMessagesFromWorker1);
  std::thread worker2(SendMessagesFromWorker2);
  worker1.join();
  worker2.join();
}

// TODO write consumer class (including extracting timestamps, populating messages, and then calling GenerateMessage() on them)

// Messages will need a deserialisation routine

// Plus a factory to deserialise the right type based on the message type read in

// Also test CPU affinity - i.e. kernel writes should not be happening on isolated thread
