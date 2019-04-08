#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <sys/times.h>
#include <sys/resource.h>
#include <fstream>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>

#include "head2.h"

using namespace  std;
char* getcwd(char *buffer, int length);
int main()
{

	string  a;
	for(;;){
		signal(SIGINT, sh1);

		char buff[66];
		if(getcwd(buff, sizeof(buff))==NULL){
			perror("ошибка getcwd");
		}else{
			printf("%s", buff);
		}

		if(getuid() == 0) printf("> ");
		else printf("! ");

		vector<vector <string> > vstr;
		getline(cin, a);
		if(!cin)
		{
			if(cin.eof())
			{
				cout << endl;
				break;
			}
			else
				cout << "other failure\n";
		}

		pars(vstr, a);

		vector<string> files(2);

		int error = zam(vstr, files);

		if(! error && vstr[0].size() > 1)
		{
			vector<pid_t> vpid;
			
			if(vstr.size()==1 && a.size() > 0 && (vstr[0][1] == "cd" || vstr[0][1] == "pwd")) vf(vstr[0]);
			else if(vstr.size() == 1 && a.size() > 0)
			{
				pid_t pid = fork();
				if(pid == 0)
				{
					if(! files[0].empty())
					{
						close(0);
						open(files[0].c_str(), O_RDWR|O_CREAT, 0666);
					}
					if(! files[1].empty())
					{
						close(1);
						open(files[1].c_str(), O_RDWR|O_CREAT|O_TRUNC, 0666);
					}

					vf(vstr[0]);
					exit(0);
				}
				else
				{
					vpid.push_back(pid);
				}
			}
			else if(a.size() > 0 && vstr.size() > 1)
			{
				int fd[vstr.size()-1][2];	
				pipe(fd[0]);
				pid_t pid = fork();
				if(pid != 0)
				{
					vpid.push_back(pid);	
				}
				else
				{
					if(! files[0].empty())
					{
						close(0);
						open(files[0].c_str(), O_RDWR|O_CREAT, 0666);
					}
					close(1);
					close(fd[0][0]);
					dup(fd[0][1]);
					vf(vstr[0]);
					exit(0);

				}
				for(int i = 1; i < vstr.size()-1;i++)
				{
					if(pid != 0)
					{
						pipe(fd[i]);
						pid = fork();
						if(pid != 0) 
						{
							/////
							close(fd[i-1][0]);
							close(fd[i-1][1]);
							/////
							vpid.push_back(pid);
						}
					}
					if(pid == 0)
					{
						close(0);
						close(fd[i-1][1]);
						dup2(fd[i-1][0], 0);
						close(1);
						close(fd[i][0]);
						dup2(fd[i][1],1);

						vf(vstr[i]);	
					
						exit(0);
					}		
				}
				if(pid != 0)
				{
					pid = fork();
					if(pid != 0) vpid.push_back(pid);
				}
				if(pid == 0)
				{
					if(! files[1].empty())
					{
						close(1);
						open(files[1].c_str(), O_RDWR|O_CREAT|O_TRUNC, 0666);
					}
					close(0);
					close(fd[vstr.size()-2][1]);
					dup(fd[vstr.size()-2][0]);
					vf(vstr[vstr.size()-1]);
					exit(0);
				}

				close(fd[vstr.size()-2][0]);
				close(fd[vstr.size()-2][1]);

			}
			
			if(vstr.size() == 1 && a.size() > 0 && vstr[0][1] == "time")
			{
				int code;
				wait(&code);

				struct rusage usage;
				getrusage(RUSAGE_SELF, &usage);
				struct timeval uend, send;
				send = usage.ru_stime;
				uend = usage.ru_utime;
				int t = clock();
				cerr << "real " << ((float)t)/ CLOCKS_PER_SEC << " s" << endl;
				cerr << "system " << send.tv_usec << " ms" << endl;
				cerr << "user " << uend.tv_usec << " ms" << endl;
			}
			else 
			{	
				
				for(int i = 0;i < vpid.size();i++)
				{
					int code;
					waitpid(vpid[i],&code,0);
				}
			}
		
		}

	}
	
	return 0;	
}	
