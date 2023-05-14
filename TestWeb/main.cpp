#include <iostream> 
#include <fstream> 
#include <string> 
#include <winsock2.h> 
#pragma comment(lib, "ws2_32.lib") 
using namespace std; 

int main()
{
	// ��ʼ��Winsock
	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != NO_ERROR) {
		cout << "WSAStartup failed: " << iResult << endl;
		return 1;
	}
	// ��������socket
	SOCKET listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	// �󶨼���socket��������ַ�Ͷ˿� 
	sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serverAddr.sin_port = htons(8888); // ����HTTPĬ�϶˿�8888 
	iResult = bind(listenSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr));
	if (iResult == SOCKET_ERROR) {
		cout << "bind failed with error: " << WSAGetLastError() << endl;
		closesocket(listenSocket);
		WSACleanup();
		return 1;
	}
	// ��ʼ���� 
	iResult = listen(listenSocket, SOMAXCONN);
	if (iResult == SOCKET_ERROR) {
		cout << "listen failed with error: " << WSAGetLastError() << endl;
		closesocket(listenSocket);
		WSACleanup();
		return 1;
	}
	cout << "���������������ȴ�������..." << endl;

	// ���Ͻ��ܿͻ����������󲢴������� 
	while (true) {
		// ���ܿͻ�����������
		SOCKET clientSocket = accept(listenSocket, NULL, NULL);
		if (clientSocket == INVALID_SOCKET) {
			cout << "accept failed with error: " << WSAGetLastError() << endl;
			closesocket(listenSocket);
			WSACleanup();
			return 1;
		}
		cout << "�����ӿͻ���" << endl; // ѭ�����տͻ��˷��������� 
		char recvBuffer[4096] = {0};
		iResult = recv(clientSocket, recvBuffer, sizeof(recvBuffer), 0);
		if (iResult == SOCKET_ERROR) {
			cout << "recv failed with error: " << WSAGetLastError() << endl;
			closesocket(clientSocket);
			WSACleanup();
			continue;
		}

		cout << recvBuffer << endl; /////////


		string request(recvBuffer, recvBuffer + iResult);

		// ��ȡHTTP�����е���Դ·�� 
		size_t start = request.find(' ') + 1;
		size_t end = request.find(' ', start);
		string path = request.substr(start, end - start);
		// ����Ĭ����ҳ���ض���index.html 
		if (path == "/") {
			path = "/login.html";
		}
		// ����HTTP��Ӧͷ 
		string responseHeader = "HTTP/1.1 200 OK\r\n"
			"Content-Type: ";
		string contentType;
		if (path.find(".html") != string::npos || path.find(".htm") != string::npos) {
			contentType = "text/html";
		}
		else if (path.find(".css") != string::npos) {
			contentType = "text/css";
		}
		else if (path.find(".js") != string::npos) {
			contentType = "application/javascript";
		}
		else if (path.find(".png") != string::npos) {
			contentType = "image/png";
		}
		else if (path.find(".jpg") != string::npos || path.find(".jpeg") != string::npos) {
			contentType = "image/jpeg";
		}
		else {
			contentType = "application/octet-stream"; // ������������
		}
		responseHeader += contentType;
		responseHeader += "\r\n"
			"Connection: close\r\n"
			"\r\n";

		iResult = send(clientSocket, responseHeader.c_str(), responseHeader.length(), 0);
		if (iResult == SOCKET_ERROR) {
			cout << "send failed with error: " << WSAGetLastError() << endl;
			closesocket(clientSocket); WSACleanup(); continue;
		}

		// ����HTTP��Ӧ���� 
		ifstream file("www" + path, ios::binary);
		if (file.fail()) {
			cout << "��ȡ�ļ�ʧ�ܣ�" << path << endl;
			/*closesocket(clientSocket);
			WSACleanup();*/
			continue;
		}

		char buffer[4096];
		while (!file.eof()) {
			file.read(buffer, sizeof(buffer));
			iResult = send(clientSocket, buffer, file.gcount(), 0);
			if (iResult == SOCKET_ERROR) {
				cout << "send failed with error: " << WSAGetLastError() << endl;
				closesocket(clientSocket);
				WSACleanup();
				break;
			}
		}

		cout << "�ѷ���HTTP��Ӧ��" << path << endl; // �ر��ļ���socket 
		file.close();
		closesocket(clientSocket);
	}

	closesocket(listenSocket);
	WSACleanup();

	return 0;

}