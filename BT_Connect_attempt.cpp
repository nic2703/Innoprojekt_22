#include <winsock2.h>
#include <ws2bth.h>
#include <stdio.h>
#include <iostream>

int main(){
  SOCKADDR_BTH sockAddr;
  SOCKET btSocket;
  WSADATA wsaData;

  int error, iResult;

  iResult = WSAStartup(MAKEWORD(2,2), &wsaData); //initialize WSA, bc otherwise it complaiins

  btSocket = socket(AF_BTH, SOCK_STREAM, BTHPROTO_RFCOMM); //define the socket with Address family bluetooth, a stream ability and the RFCOMM protocol
  memset((&sockAddr), 0, sizeof(sockAddr)); //set the momory of sockAddr to all 0, to avoid undefined behaviour
  sockAddr.addressFamily = AF_BTH;
  //sockAddr.serviceClassId = RFCOMM_PROTOCOL_UUID; 
  //RFCOMM protocol Breaks thing?

  sockAddr.port = BT_PORT_ANY; //connect using any port
  sockAddr.btAddr = BTH_ADDR(0x4851c56b31b2); //some address here, can you figure out the format necessary?
  error = connect(btSocket, (SOCKADDR*)&sockAddr, sizeof(sockAddr)); //connect the port to the specified locations
  
  std::cout << WSAGetLastError() << " " << error << std::endl; //uhhh, debugging lol
  
  closesocket(btSocket); // im sure this is all the security we need, maybe also get the return value from this and see the value at some point, 0 is exectuted correctly, else not
  //alternatively use WSAGetLastError() to find out what error closesocket() throws, if any

  return 0; // need i explain this? (jk)
}
