



#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <wait.h>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <vector>
#include <iterator>
#include <functional>
#include <stack>
using namespace std;

struct simulated_process
{
	int id;
	int completion_time;
	int deadline;
	bool running;
	stack<string> command_stack;
};

class system_snapshot
{
	public:
		int num_resources;
		int num_processes;
		int* available;
		simulated_process* processes;
		int** max;
		int** allocated;
		int** need;
};

struct two_way_pipe
{
	int child_to_parent[2];
	int parent_to_child[2];
};


system_snapshot read_input(const string &input_file)
{
	
	int num_resources;
	int num_processes;
	int* available;
	simulated_process* processes;
	int** max;
	int** allocated;
	int** need;
	
	
	string line;
	ifstream myfile (input_file.c_str());
	if (myfile.is_open())
	{
		getline(myfile, line);
		istringstream(line) >> num_resources;
		getline(myfile, line);
		istringstream(line) >> num_processes;
		
		
		available = new int[num_resources];
		processes = new simulated_process[num_resources];
		max = new int*[num_processes];
		for(int i = 0; i < num_processes; ++i)
			max[i] = new int[num_resources];
		
		allocated = new int*[num_processes];
		for(int i = 0; i < num_processes; ++i)
			allocated[i] = new int[num_resources];
		
		need = new int*[num_processes];
		for(int i = 0; i < num_processes; ++i)
			need[i] = new int[num_resources];
		
		
		
		for(int i = 0; i<num_resources; i++)
		{
			int index;
			int value;
			
			getline(myfile, line);
			
			size_t index1 = line.find("[");
			size_t index2 = line.find("]");
			if (index1!=string::npos and index2!=string::npos)
			{
				string tmp = line.substr(index1+1, index2-(index1+1));
				istringstream(tmp) >> index;
			}
			else
			{
				perror("error reading input file.\n");
				exit(2);
			}
			
			index1 = line.find("=");
			if(index1!=string::npos)
			{
				string tmp = line.substr(index1+1, line.length()-(index1+1));
				istringstream(tmp) >> value;
			}
			else
			{
				perror("error reading input file.\n");
				exit(2);
			}
			
			available[index-1] = value;
		}
		
		
		for(int i = 0; i<num_processes; i++)
		{
			for(int k = 0; k<num_resources; k++)
			{
				int process_index;
				int resource_index;
				int value;
				
				getline(myfile, line);
				
				size_t index1 = line.find("[");
				size_t index2 = line.find(",");
				if (index1!=string::npos and index2!=string::npos)
				{
					string tmp = line.substr(index1+1, index2-(index1+1));
					istringstream(tmp) >> process_index;
				}
				else
				{
					perror("error reading input file.\n");
					exit(2);
				}
				
				index1 = line.find(",");
				index2 = line.find("]");
				if (index1!=string::npos and index2!=string::npos)
				{
					string tmp = line.substr(index1+1, index2-(index1+1));
					istringstream(tmp) >> resource_index;
				}
				else
				{
					perror("error reading input file.\n");
					exit(2);
				}
				
				index1 = line.find("=");
				if(index1!=string::npos)
				{
					string tmp = line.substr(index1+1, line.length()-(index1+1));
					istringstream(tmp) >> value;
				}
				else
				{
					perror("error reading input file.\n");
					exit(2);
				}
				
				max[process_index-1][resource_index-1] = value;
				allocated[process_index-1][resource_index-1] = 0;
				need[process_index-1][resource_index-1] = max[process_index-1][resource_index-1] - allocated[process_index-1][resource_index-1];
			}
		}
		
		
		int count = 0;
		while(count<num_processes)
		{
			simulated_process p;
			
			
			getline(myfile, line);
			size_t index1 = line.find("_");
			if(index1!=string::npos)
			{
				string tmp = line.substr(index1+1, line.length()-(index1+1));
				istringstream(tmp) >> p.id;
			}
			else
			{
				perror("error reading input file.\n");
				exit(2);
			}
			
			
			getline(myfile, line);
			istringstream(line) >> p.deadline;
			
			getline(myfile, line);
			istringstream(line) >> p.completion_time;
			
			vector<string> temp_commands;
			
			while(true)
			{
				getline(myfile, line);
				temp_commands.push_back(line);
				size_t k = line.find("end");
				if (k!=string::npos) break;
			}
			stack<string> commands;
			while(temp_commands.size()>0)
			{
				commands.push(temp_commands.back());
				temp_commands.pop_back();
			}
			p.command_stack = commands;
			p.running = true;
			
			processes[count] = p;
			count++;
		}
	}
	else cout << "Unable to open input file"; 
	
	system_snapshot current_snapshot;
	current_snapshot.available = available;
	current_snapshot.max = max;
	current_snapshot.allocated = allocated;
	current_snapshot.need = need;
	current_snapshot.processes = processes;
	current_snapshot.num_processes = num_processes;
	current_snapshot.num_resources = num_resources;
	
	return current_snapshot;
}


simulated_process* get_next_process(system_snapshot &sys, int previous_id, bool mode)
{
	bool EDF = mode;
	bool LJF = !mode;


	int num_active_process = 0;
	for(int i = 0; i<sys.num_processes; i++)
	{
		if(sys.processes[i].running==true) num_active_process++;
	}

	simulated_process* ptr = NULL;
	for(int i = 0; i<sys.num_processes; i++)
	{
		if(sys.processes[i].running==false) continue;			
		if(sys.processes[i].id == previous_id and num_active_process>1) continue;		
		
		if(LJF == true)
		{
			if (ptr==NULL) ptr = &sys.processes[i];
			else if (sys.processes[i].completion_time > ptr->completion_time)
			{
				ptr = &sys.processes[i];
			}
			else if (sys.processes[i].completion_time == ptr->completion_time and sys.processes[i].deadline < ptr->deadline)
			{
				ptr = &sys.processes[i];
			}
		}
		else if(EDF == true)
		{
			if (ptr==NULL) 
			{
				ptr = &sys.processes[i];
			}
			else if (sys.processes[i].deadline < ptr->deadline)
			{
				ptr = &sys.processes[i];
			}
			else if (sys.processes[i].deadline == ptr->deadline and sys.processes[i].completion_time > ptr->completion_time)
			{
				ptr = &sys.processes[i];
			}
		}
	}
	
	if (ptr == NULL)
	{
		cout << "Active process is NULL pointer, either done or error." << endl;
	}
	
	return ptr;
}

two_way_pipe start_process(simulated_process &p, system_snapshot &sys)
{
	two_way_pipe comm;
	
	pid_t process_id;
	
	if (pipe(comm.child_to_parent) == -1 or pipe(comm.parent_to_child) == -1)
	{
		perror("error creating pipe");
		exit(2);
	}
	
	process_id = fork();
	if(process_id == -1)
	{
		perror("error creating process using fork");
		exit(2);
	}
	
	if(process_id==0) 
	{
		char delim[1];
		delim[0] = '|';
		
		while(p.command_stack.size()>0)
		{
			
			string cmd = p.command_stack.top();
			
			char c[20];
			sprintf(c, "%d", p.id);
			
			write(comm.child_to_parent[1], c, 1);
			write(comm.child_to_parent[1], ":", 1);
			write(comm.child_to_parent[1], cmd.c_str(), cmd.length()+1);
			write(comm.child_to_parent[1], delim, sizeof(delim));
			
			
			
			int signal;
			read(comm.parent_to_child[0], &signal, sizeof(signal));
			if (signal==0)
			{
				
			}
			else
			{
				
				p.command_stack.pop();
			}
		}

		exit(0);
	}
	

	return comm;
}

string fetch_command(int pipe_open, char delim)
{
	string tmp;
	char buff[1];
	while(true)
	{
		read(pipe_open, buff, 1);
		if(buff[0]!=delim)
		{
			tmp = tmp + buff[0];
		}
		else break;
	}
	return tmp;
}

bool safe(system_snapshot &sys)
{
	int* work = new int[sys.num_resources];
	bool* finish = new bool[sys.num_processes];
	
	for(int i = 0; i<sys.num_resources; i++)
		work[i] = sys.available[i];
	for(int i = 0; i<sys.num_processes; i++)
		finish[i] = false;
		
	bool complete = false;
	while(complete == false)
	{
		complete = true;
		for(int i = 0; i<sys.num_processes; i++)
		{
			
			if(sys.processes[i].running==false) continue;
			
			
			bool check_all_resources = true;
			for(int k = 0; k<sys.num_resources; k++)
			{
				if(finish[i]==false and sys.need[i][k]>work[k])
				{
					check_all_resources = false;
				}
			}
			
			if(check_all_resources == true and finish[i]!=true)
			{
				for(int k = 0; k<sys.num_resources; k++)
				{
					work[k] = work[k] + sys.allocated[i][k];
				}
				finish[i] = true;
				complete = false;
			}
		}
	}
	
	
	bool is_safe = true;
	for(int i = 0; i<sys.num_processes; i++)
	{
		if(sys.processes[i].running==false) continue;
		if(finish[i]==false)	is_safe = false;
	}
	
	return is_safe;
}

int bankers_algorithm(system_snapshot &sys, string request, int process_id, string &status)
{
	
	int resource = 0;
	int* request_vector = new int[sys.num_resources];
	int* release_vector = new int[sys.num_resources];
	size_t left_paren = request.find("(");
	size_t release = request.find("release");
	if(left_paren!=string::npos)
	{
		string tmp = request.substr(left_paren+1, 2*sys.num_resources-1);
		for(int i = 0; i<2*sys.num_resources-1; i=i+2)
		{
			string val = tmp.substr(i, 1);
			istringstream ( val ) >> request_vector[resource];
			istringstream ( val ) >> release_vector[resource];
			resource++;
		}
	}
	else
	{
		
		size_t end = request.find("end");
		if(end!=string::npos)
		{
			
			for(int i = 0; i<sys.num_resources; i++)
			{
				sys.available[i] = sys.available[i] + sys.allocated[process_id][i];
				sys.allocated[process_id][i] = 0;
				sys.need[process_id][i] = sys.max[process_id][i];
			}
			sys.processes[process_id].running = false;

			return 0;
		}
		else
		{
			status = "Unable to understand request";
			return -2;
		}
	}
	if(release!=string::npos)
	{
		
		bool safe_to_release = true;
		
		for(int i = 0; i<sys.num_resources; i++)
		{
			int avail_tmp = 0;
			int alloc_tmp = 0;
			int need_tmp = 0;
		
			avail_tmp = sys.available[i] + release_vector[i];
			alloc_tmp = sys.allocated[process_id][i] - release_vector[i];
			need_tmp = sys.need[process_id][i] + release_vector[i];
			
			if(avail_tmp <0 or alloc_tmp<0 or need_tmp<0) 
				safe_to_release = false;
		}
		if(safe_to_release == true)
		{
			for(int i = 0; i<sys.num_resources; i++)
			{
				sys.available[i] = sys.available[i] + release_vector[i];
				sys.allocated[process_id][i] = sys.allocated[process_id][i] - release_vector[i];
				sys.need[process_id][i] = sys.need[process_id][i] + release_vector[i];
			}
			return 0;
		}
		else
		{
			status = "Unable to release resources. Release attempted to release more resources than the process had allocated.";
			return -2;
		}
	}
	
	
	for(int i = 0; i<sys.num_resources; i++)
	{
		if (request_vector[i] > sys.need[process_id][i])
		{
			status = "Process requested more resources than initially described in max. Error.";
			return -2;
		}
		else if(request_vector[i] > sys.available[i])
		{
			return 1;
		}
	}
	for(int i = 0; i<sys.num_resources; i++)
	{
		
		sys.available[i] = sys.available[i] - request_vector[i];
		sys.allocated[process_id][i] = sys.allocated[process_id][i] + request_vector[i];
		sys.need[process_id][i] = sys.need[process_id][i] - request_vector[i];
	}
	
	bool is_alloc_safe = safe(sys);
	if(is_alloc_safe==false)
	{
		status = "Unsafe state detected. Forcing process to wait.";
		for(int i = 0; i<sys.num_resources; i++)
		{
			sys.available[i] = sys.available[i] + request_vector[i];
			sys.allocated[process_id][i] = sys.allocated[process_id][i] - request_vector[i];
			sys.need[process_id][i] = sys.need[process_id][i] + request_vector[i];
		}
		return 1;
	}

	return 0;
}

void print_state(system_snapshot &sys, string cmd, int proc_id, string &status)
{
	cout << endl;
	cout << "Executing command: " << cmd << endl;
	cout << "Result: " << status << endl << endl;
	cout << "System Information:" << endl;
	cout << "Available\t" << "Need["<<proc_id+1<<"]\t" << "Allocated["<<proc_id+1<<"]\t" << endl;
	for(int i = 0; i<sys.num_resources; i++)
		cout << sys.available[i] << "\t\t" << sys.need[proc_id][i] << "\t\t" << sys.allocated[proc_id][i] << endl;
	cout << endl;
	cout << "Process Information["<<proc_id+1<<"]:" << endl;
	cout << "Deadline:\t\t" << sys.processes[proc_id].deadline << endl;
	cout << "Completion Time:\t" << sys.processes[proc_id].completion_time << endl << endl;
}

int main(int argc, char *argv[])
{
	string input = "input.txt";
	bool mode = true;
	if (argv[1]!=NULL)
	{
		if(argv[1][0] == 'T')	mode = true;
		if(argv[1][0] == 'F') 	mode = false;
	}

	int total_time = 0;
	int next_id = -1;
	system_snapshot system = read_input(input);
	two_way_pipe * pipes = new two_way_pipe[system.num_processes];
	
	for(int i = 0; i<system.num_processes; i++)
	{
		pipes[i] = start_process(system.processes[i], system);
	}
	
	bool active_processes = true;
	while(active_processes == true)
	{
		total_time++;
		
		active_processes = false;
		for(int i = 0; i<system.num_processes; i++)
			if(system.processes[i].running == true) active_processes = true;
		if(active_processes == false) break;

		
		simulated_process* ptr = get_next_process(system, next_id, mode);
		next_id = ptr->id;

		
		
		for(int i = 0; i<system.num_processes; i++)
		{
			
			if(system.processes[i].running==false) continue;
			
			
			string cmd = fetch_command(pipes[i].child_to_parent[0], '|');
			size_t index = cmd.find(":");
			string tmp_id = cmd.substr(0, index);
			int proc_id;
			istringstream(tmp_id) >> proc_id;
			
			
			int signal = 0;
			if (proc_id == next_id) 
			{
				signal = 1;
				
			
				
				
				
				
				
				string status = "success";
				int safe = bankers_algorithm(system, cmd, proc_id-1, status);
				switch(safe)
				{
					case(1): signal = 0; print_state(system, cmd, proc_id-1, status); break;
					case(0): print_state(system, cmd, proc_id-1, status); break;
					case(-1): print_state(system, cmd, proc_id-1, status); break;
					case(-2): print_state(system, cmd, proc_id-1, status); break;
					default: break;
				}
				
				
				
				system.processes[i].completion_time--;
				system.processes[i].deadline--;
				
			}
			else
			{
				system.processes[i].deadline--;
			}
			size_t chk = cmd.find("end");
			if(chk!=string::npos)
			{
				system.processes[i].running = false;
			}
			
			write(pipes[i].parent_to_child[1], &signal, sizeof(signal));
		}




	}
	int num_deadlines_missed  = 0;
	for(int i = 0; i<system.num_processes; i++)
	{
		if(system.processes[i].deadline<0) num_deadlines_missed++;
	}
	cout << endl << endl;
	cout << "Complete";
	if(mode==true) cout << " (EDF):" << endl;
	if(mode==false) cout << " (LJF):" << endl;
	cout << "\tTotal time taken:\t\t" << total_time << endl;
	cout << "\tNumber of deadlines missed:\t" << num_deadlines_missed << endl;

	return 0;
}
