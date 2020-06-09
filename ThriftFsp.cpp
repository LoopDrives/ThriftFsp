// ThriftFsp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <ThriftFuse.h>
#include "FuseImpl.h"
#include <thrift/transport/TTransportUtils.h>
#include <thrift/transport/TPipe.h>
#include <thrift/transport/TSocket.h>

#include <thrift/protocol/TBinaryProtocol.h>

using namespace std;
using namespace apache::thrift;
using namespace apache::thrift::transport;
using namespace apache::thrift::protocol;

int main()
{
    apache::thrift::GlobalOutput;
    string endpoint;
#ifdef _WIN32
    std::shared_ptr<TTransport> trasnPort(new TPipe(endpoint.c_str()));
#elif
    std::shared_ptr<TTransport> trasnPort(new TSocket(endpoint.c_str()));
#endif
 
    std::shared_ptr<TProtocol> transportProtocol(new TBinaryProtocol(trasnPort));
    ThriftFuseClient client(transportProtocol);

    auto instance = FuseImpl::GetInstance();
    instance->Main(0,nullptr, &client);
}

