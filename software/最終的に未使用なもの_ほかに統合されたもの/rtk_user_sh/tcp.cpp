#include "tcp.h"


//�z�X�g����IP�A�h���X����ڑ���TCP�s��  (*���e�X�g)
//����1=�z�X�g��(IP�A�h���X)�A����2=�|�[�g�ԍ�
void tcpClient::access(char hostname_str[], char port_str[])
{
	char temp[256];
	hostname2ip(hostname_str, temp);
	myConnect(temp, port_str);
}

//�֐��錾���̃��[�h�ݒ�
//����=���[�h
tcpClient::tcpClient(int mode)
{
	nowMode = mode;
}

//�錾�ς݊֐��̃��[�h�ύX
//����=���[�h
void tcpClient::setMode(char wantMode)
{
	nowMode = wantMode;
}

//recvBuffer���ɂ��镶�����ݒ肳�ꂽ���[�h�ɏ]���āAreadBuffer�Ɋi�[
void tcpClient::scan()
{
	switch (nowMode)
	{
	case MODE_READ_SPEED:
		scanG();
		break;
	case MODE_READ_RESULT:
		scanRaw();
		break;
	default:
		break;
	}
}

//readBuffer���̕��������؂蕶�����ƂɔF�����A�ݒ胂�[�h���Ƃɕϐ��Ɋi�[
//����=1�̎��A��؂蕶���ŋ�؂������ʂ�\���A0=��؂蕶���ŋ�؂������ʂ��\��
void tcpClient::print_strtokc(int printFlg)
{
	switch (nowMode)
	{
	case MODE_READ_SPEED:
		printG_strtokc(printFlg);
		break;
	case MODE_READ_RESULT:
		printR_strtokc(printFlg);
		break;
	default:
		break;
	}
}

//readBuffer���̕��������؂蕶�����ƂɔF�����A�ܓx�A�o�x�A�����A�v�Z���x��ϐ��Ɋi�[
//����=1�̎��A��؂蕶���ŋ�؂������ʂ�\���A0=��؂蕶���ŋ�؂������ʂ��\��
void tcpClient::printR_strtokc(int printFlg)
{
	char temp[BUFSIZ];
	char s2[] = " ";
	char *tok;

	if (printFlg == 1)
	{
		printf("------------------\n");
		print();
		printf("------------------\n");
	}
	if (readFlg == 0)
	{
		int num = 0;
		tok = strtok(readBuffer, s2);
		while (tok != NULL)
		{
			if (num == 2)
			{
				lat = atof(tok);
			}
			else if (num == 3)
			{
				lon = atof(tok);
			}
			else if (num == 4)
			{
				hei = atof(tok);
			}
			else if (num == 14)
			{
				ratio = atof(tok);
			}

			num++;
			if (printFlg == 1)
				printf("%s\n", tok);
			tok = strtok(NULL, s2);
		}
		if (printFlg == 1)
			printf("%d------------------\n\n", num);
	}
}

//readBuffer���̕��������؂蕶�����ƂɔF�����A�ړ������ƈړ����x��ϐ��Ɋi�[
//����=1�̎��A��؂蕶���ŋ�؂������ʂ�\���A0=��؂蕶���ŋ�؂������ʂ��\��
void tcpClient::printG_strtokc(int printFlg)
{
	char temp[BUFSIZ];
	char s2[] = ",";
	char *tok;

	int vtgFlg = 0;

	if (printFlg == 1)
	{
		printf("------------------\n");
		print();
		printf("------------------\n");
	}
	if (readFlg == 0)
	{
		int num = 0;
		tok = myStrtok(readBuffer, s2);
		vtgFlg = strcmp(tok, GNVTG_STR);
		while (tok != NULL)
		{
			if (vtgFlg == 0)
			{
				if (num == 3)
				{
					angele = atof(tok);
				}
				else if (num == 7)
				{
					speed = atof(tok);
				}
			}
			num++;
			if (printFlg == 1)
				printf("%s\n", tok);
			tok = myStrtok(NULL, s2);
		}
		if (printFlg == 1)
			printf("%d------------------\n\n", num);
	}
}

/*
strtok�֐��̋�؂蕶���̘A���������Ƃ��Ȃ��o�[�W����
*/
char *tcpClient::myStrtok(char *s1, char *s2)
{
	static char *str = 0;

	if (s1)
	{
		str = s1;
	}
	else
	{
		s1 = str;
	}
	if (!s1)
		return (0);

	while (1)
	{
		if (!*str)
		{
			str = 0;
			return (s1);
		}

		if (*str == *s2)
		{
			*str++ = 0;
			return (s1);
		}

		str++;
	}
}

//�A�X�L�[�R�[�h�̊m�F
//�߂�l�F1=���s�Ao=����
int tcpClient::ascCheck(char c)
{
	if ((c >= 0x20) && (c < 0x7f))
	{
		return 0;
	}
	return 1;
}

//�z�X�g������IP�A�h���X�擾
//����1==�z�X�g��(IP�A�h���X)�A����2=�������ʕۑ��p������
void tcpClient::hostname2ip(char hostname_str[], char ip_str[])
{
	struct sockaddr_in addr;
	struct hostent *host;
	host = gethostbyname(hostname_str);
	if (host != NULL)
	{
		addr.sin_addr = *(struct in_addr *)(host->h_addr_list[0]);
	}
	sprintf(ip_str, "%s", inet_ntoa(addr.sin_addr));
}

//IP�A�h���X�ƃ|�[�g�ԍ����Q�l��TCP�ڑ�
//����1=IP�A�h���X�A����2=�|�[�g�ԍ�
void tcpClient::myConnect(char IP_str[], char port_str[])
{
	memset(&servSockAddr, 0, sizeof(servSockAddr));

	servSockAddr.sin_family = AF_INET;

	if (inet_aton(IP_str, &servSockAddr.sin_addr) == 0)
	{
		fprintf(stderr, "Invalid IP Address.\n");
		exit(EXIT_FAILURE);
	}

	if ((servPort = (unsigned short)atoi(port_str)) == 0)
	{
		fprintf(stderr, "invalid port number.\n");
		exit(EXIT_FAILURE);
	}
	servSockAddr.sin_port = htons(servPort);

	if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
	{
		perror("socket() failed.");
		exit(EXIT_FAILURE);
	}

	if (connect(sock, (struct sockaddr *)&servSockAddr, sizeof(servSockAddr)) < 0)
	{
		perror("connect() failed.");
		exit(EXIT_FAILURE);
	}

	printf("connect to %s\n", inet_ntoa(servSockAddr.sin_addr));
}


//�ڑ��ς�TCP�\�P�b�g���當�����ǂݎ��
int tcpClient::read()
{

	if (send(sock, sendBuffer, strlen(sendBuffer), 0) <= 0)
	{
		perror("send() failed.");
		exit(EXIT_FAILURE);
	}

	int byteRcvd = 0;
	int byteIndex = 0;
	while (byteIndex < MSGSIZE)
	{
		byteRcvd = recv(sock, &recvBuffer[byteIndex], 1, 0);
		if (byteRcvd > 0)
		{
			if (nowMode == MODE_READ_SPEED)
			{
				//if ((recvBuffer[byteIndex] == '\n')||(recvBuffer[byteIndex] == '\r'))
				if (recvBuffer[byteIndex] == '\n')
				{
					recvBuffer[byteIndex] = '\0';

					/*if (strcmp(recvBuffer, "quit") == 0)
                                {
                                        close(sock);
                                        return EXIT_SUCCESS;
                                }
                                else
                                {
                                        break;
                                }*/
					break;
				}
			}
			else
			{
				if (recvBuffer[byteIndex] == '\n')
				{
					recvBuffer[byteIndex] = '\0';
					break;
				}
			}

			byteIndex += byteRcvd;
		}
		else if (byteRcvd == 0)
		{
			perror("ERR_EMPTY_RESPONSE");
			exit(EXIT_FAILURE);
		}
		else
		{
			perror("recv() failed.");
			exit(EXIT_FAILURE);
		}
	}
	return 0;
}

//recvBuffer���ɂ��镶������ANMEA�t�H�[�}�b�g��ǂݎ��`����readBuffer�Ɋi�[
//ubox�`���ƘA�������ڂ�NMEA�t�H�[�}�b�g�̕�����͓ǂݎ��Ȃ�(GPRMC�Ȃ�)
void tcpClient::scanG()
{
	char *adr;
	char str[] = "$G";
	int i;
	adr = strstr(recvBuffer, str);
	//printf("server return: %s\n", recvBuffer);
	if (adr != NULL)
	{
		int j = 0;
		while (*(adr + j) != '\0')
		{
			if (ascCheck(*(adr + j)) == 0)
				readBuffer[j] = *(adr + j);
			j++;
		}
		readBuffer[j] = '\0';
		readFlg = 0;
	}
	else
	{
		readFlg++;
	}
}

//�A�X�L�[�R�[�h�̊m�F�����s���ArecvBuffer���ɂ��镶�����readBuffer�Ɋi�[
void tcpClient::scanRaw()
{
	int i;
	//printf("server return: %s\n", recvBuffer);
	int j = 0;
	while (recvBuffer[j] != '\0')
	{
		if (ascCheck(recvBuffer[j]) == 0)
			readBuffer[j] = recvBuffer[j];
		j++;
	}
	readBuffer[j] = '\0';
	readFlg = 0;
}

//readBuffer���ɍŐV�̕����񂪂���΂����\�����A�Ȃ���΁A���s�\��
void tcpClient::print()
{
	if (readFlg == 0)
	{
		//printf("------------------\n");
		printf("%s\n", readBuffer);
		//printf("------------------\n");
	}
	else if (readFlg == 1)
		printf("\n");
}

