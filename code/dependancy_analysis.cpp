#include <iostream>
#include <stdio.h>
#include <string>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <string.h>
#include <algorithm>
#include <vector> 


using namespace std;


class instruction_table
{
	public:
		string opcode;
		vector<string> operands;
		string dest_operand; 
		string source_operand_1;
		string source_operand_2;
};

void print_table(instruction_table * ins_table , int line_count)
{
	int i=0;
	for(i=0;i<line_count;i++)
	{
		cout<<"instuction : "<<ins_table[i].opcode <<" --> "<< ins_table[i].operands[0]<<" , "<<ins_table[i].operands[1]<<" , "<<ins_table[i].operands[2]<<"\n";
	}
}

int check_RAW_hazard(instruction_table * ins_table,int line_count)
{
	int number_of_RAW_hazards=0;
	int i=0,j=0;
	for(i=0;i<(line_count-4);i++)
	{
		for(j=i+1;j<(i+4);j++)
		{
			if(ins_table[i].dest_operand == ins_table[j].source_operand_1 || ins_table[i].dest_operand == ins_table[j].source_operand_2)
			{
					number_of_RAW_hazards++;

			}
		}
	}
	return number_of_RAW_hazards;
}

int check_WAR_hazard(instruction_table * ins_table,int line_count)
{
	int number_of_WAR_hazards=0;
	int i=0,j=0;
	for(i=0;i<(line_count-4);i++)
	{
		for(j=i+1;j<(i+4);j++)
		{
			if(ins_table[i].source_operand_1 == ins_table[j].dest_operand || ins_table[i].source_operand_2 == ins_table[j].dest_operand)
			{
				number_of_WAR_hazards++;
			}
		}
	}
	return number_of_WAR_hazards;
}

int check_WAW_hazard(instruction_table * ins_table,int line_count)
{
	int number_of_WAW_hazards=0;
	int i=0,j=0;
	for(i=0;i<(line_count-4);i++)
	{
		for(j=i+1;j<(i+4);j++)
		{
			if(ins_table[i].dest_operand == ins_table[j].dest_operand)
			{
				number_of_WAW_hazards++;
			}
		}
	}
	return number_of_WAW_hazards;
}

int compute_data_dependancies(instruction_table * ins_table, int line_count)
{
	int number_of_data_dependancies=0;
	int i=0,j=0;
	for(i=0;i<line_count;i++)
	{
		for(j=i+1;j<(line_count);j++)
		{
			if(ins_table[i].dest_operand == ins_table[j].source_operand_1 || ins_table[i].dest_operand == ins_table[j].source_operand_2)
			{
					number_of_data_dependancies++;

			}
		}
	}
	return number_of_data_dependancies;
}

int compute_anti_dependancies(instruction_table * ins_table, int line_count)
{
	int number_of_anti_dependancies=0;
	int i=0,j=0;
	for(i=0;i<line_count;i++)
	{
		for(j=i+1;j<(line_count);j++)
		{
			if(ins_table[i].source_operand_1 == ins_table[j].dest_operand || ins_table[i].source_operand_2 == ins_table[j].dest_operand)
			{
				number_of_anti_dependancies++;
			}
		}
	}
	return number_of_anti_dependancies;
}

int compute_output_dependancies(instruction_table * ins_table, int line_count)
{
	int number_of_output_dependancies=0;
	int i=0,j=0;
	for(i=0;i<line_count;i++)
	{
		for(j=i+1;j<line_count;j++)
		{
			if(ins_table[i].dest_operand == ins_table[j].dest_operand)
			{
				number_of_output_dependancies++;
			}
		}
	}
	return number_of_output_dependancies; 
}




int main()
{

string sub1,sub2,wt1,wt2,wt3;
string line;
ifstream file( "input1.txt" );
ifstream file1( "input1.txt" );
int line_count=0;
int label_count=0;
int index=0;
instruction_table *ins_table;
int i=0,j=0;

int number_of_data_dependancies=0;
int number_of_anti_dependancies=0;
int number_of_output_dependancies=0;

int number_of_RAW_hazards=0;
int number_of_WAR_hazards=0;
int number_of_WAW_hazards=0;



if(file1)
{
	while(getline(file1,line))
	{
		line_count++;
	}
	file1.close();
}

cout<<"line count : "<<line_count<<"\n";

ins_table = new instruction_table[line_count];

if(file)
{
	while(getline(file,line))
	{

	//	cout<<"line : "<<line<<"\n";

		istringstream iss(line);
		
			
			iss >> sub1;
			//cout<<sub1<<"\n";
			//input_table.push_back(sub1);

			if(sub1 != "L1")
			{

			label_count++;
			//cout<<"caught\n";
			ins_table[index].opcode = sub1;

			iss >> sub2;
			//cout<<sub2<<"\n";
			
			replace(sub2.begin(), sub2.end(), ',', ' ');
			istringstream iss1(sub2);
			iss1 >> wt1; 
			iss1 >> wt2;
			iss1 >> wt3;

			//	cout<< wt1<<"\n";
			//	cout<<wt2<<"\n";
			//	cout<<wt3<<"\n";

			ins_table[index].operands.push_back(wt1);
			ins_table[index].dest_operand = wt1;
			ins_table[index].operands.push_back(wt2);
			ins_table[index].source_operand_1 = wt2;
			ins_table[index].operands.push_back(wt3);
			ins_table[index].source_operand_2 = wt3;

			index++;
		}
	}
	file.close();
}

line_count = label_count;

// print instruction table

	print_table(ins_table,line_count);

	number_of_data_dependancies = compute_data_dependancies(ins_table,line_count);

	number_of_anti_dependancies = compute_anti_dependancies(ins_table, line_count);

	number_of_output_dependancies = compute_output_dependancies(ins_table, line_count);

	number_of_RAW_hazards = check_RAW_hazard(ins_table,line_count);

	number_of_WAR_hazards = check_WAR_hazard(ins_table,line_count);

	number_of_WAW_hazards = check_WAW_hazard(ins_table,line_count);

	cout<<"total number of data dependancies : "<< number_of_data_dependancies<<"\n";

	cout<<"total number of anti dependancies : "<<number_of_anti_dependancies<<"\n";

	cout<<"total number of output dependancies : "<<number_of_output_dependancies<<"\n";

	cout<<"total number of RAW hazards : "<< number_of_RAW_hazards <<"\n";

	cout<<"total number of WAR hazards : "<< number_of_WAR_hazards <<"\n";

	cout<<"total number of WAW hazards : "<< number_of_WAW_hazards <<"\n";

	return 0;
}