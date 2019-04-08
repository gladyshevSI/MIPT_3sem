#include"head2.h"


using namespace std;


void sh1(int signum) {}


void pars(vector<vector<string> >& vstr, string str)
{
	vector<string> nn = {" "};
	vstr.push_back(nn);

	long long n = str.size();
	string now = ""; 

	long long j = 0;

	for(long long i = 0; i < n; ++i)
	{
		if(str[i] != ' ' && str[i] != '\t' && str[i] != '|' && str[i] != '<' && str[i] != '>')
		{
			now += str[i];
		}
		else if((str[i] == ' ' || str[i] == '\t') && now != "")
		{
			vstr[j].push_back(now);
			now = "";
		}
		else if(str[i] == '|' || str[i] == '<' || str[i] == '>')
		{
			if(now != "")
			{
				vstr[j].push_back(now);
				now = "";
			}
			
			string a = "";
		       	a += str[i];
			vector<string> b = {a};
			vstr.push_back(b);

			j++;
		}
	}
	if(now != "")
	{
		vstr[j].push_back(now);
	}
	

}

void vf(vector<string>& vstri)
{
	if(vstri[1] == "cd")
	{
		if(2<vstri.size())
		{
			cd(vstri[2]);
		}
		else
		{
			string scd = "/home";
			cd(scd);
		}
	}
	else if(vstri[1] == "pwd")
	{
		pwd();
	}
	else if(vstri[1] == "time")
	{
		mytime(vstri);
	}
	else
	{
		shellf(vstri);
	}
}
	

void pwd()
{
	char buff[66];
	if(getcwd(buff, sizeof(buff))==NULL)
	{
		perror("ошибка getcwd");
	}else{
		cout<<buff<<endl;
	}
}


void cd(const string scd)
{
	chdir(scd.c_str());
}


void shellf(vector<string>& vstrj)
{
	char **args = new char *[vstrj.size()];
	
	for(int i = 0;i<vstrj.size()-1;i++){

		args[i] = strdup(vstrj[i+1].c_str());
	}

	args[vstrj.size()-1] = NULL;	
	
	execvp(args[0],args);

	for(int i = 0;i<vstrj.size();i++){
		free(args[i]);
	}	
	delete [] args;
}

int zam(vector<vector<string> >& vstr, vector<string>& files)
{
	vector<vector<string> > nvstr;
	for(int j = 0; j < vstr.size();j++)
	{
		if(vstr[j][0] == "<")
		{
			if(j == 1) files[0] = vstr[j][1];
			else
			{
				printf("error < \n");
				return 1;
			}
		}
		else if(vstr[j][0] == ">")
		{
			if(j == vstr.size()-1) files[1] = vstr[j][1];
			else
			{
				printf("error > \n");
				return 1;
			}
		}
		else
		{
			vector<string> newvstr;
			for(int i = 0; i < vstr[j].size();i++)
			{
				if(vstr[j][i].find("*") != -1 || vstr[j][i].find("?") != -1)
				{
					glob_t globbuf;
					glob(vstr[j][i].c_str(), GLOB_NOCHECK, NULL, &globbuf);
					
					for(int x = 0; x < globbuf.gl_pathc; x++)
					{
						string newarg = globbuf.gl_pathv[x];
						newvstr.push_back(newarg);
					}
				}
				else
				{
					newvstr.push_back(vstr[j][i]);
				}
			}
			nvstr.push_back(newvstr);
		}
	}

	vstr = nvstr;
	return 0;
}

void mytime(vector<string>& vstri)
{
	vector<string>::iterator it;
	it = vstri.begin() + 1;
	vstri.erase(it);

	vf(vstri);
}
