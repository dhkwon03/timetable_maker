/*
Copyright 2021 Doheon Kwon <doheon.kwon@gmail.com>
ALL RIGHTS RESERVED
REPRODUCTION AND MODIFICATION ONLY WITH AUTHOR'S PERMISSION
*/

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>
#include <algorithm>
#include <string>
#include <vector>

using namespace std;

typedef struct __TIME {
	int start_time;
	int end_time;
}TIME;

typedef struct __CLASS {
	char class_name[10];
	int class_time_num;
	int pref_rate;	
	vector<TIME> times;
}CLASS;

typedef struct __LECTURE {
	char lec_code[100];
	int class_num;
	vector<CLASS> classes;
}LECTURE;

typedef struct __TIMETABLE {
	vector<int> class_numbers;
	int weight;
}TIMETABLE;

int lecture_num;
vector<LECTURE> lec;
int idx[100];
vector<TIMETABLE> timetable;

int check_time_overlap(int a_st, int a_en, int b_st, int b_en)
{
	if(((a_st <= b_st) && (b_st < a_en)) || ((a_st < b_en) && (b_en <= a_en)) || ((a_st >= b_st) && (a_en <= b_en))) {
		//overlapped
		return 1;
	}	
	else {
		return 0;
	}
}

int make_timetable()
{
	int flag = 0;

	for(int i = 0; i < lecture_num; i++) {
		for(int j = (i + 1); j < lecture_num; j++) {
			for(int k = 0; k < lec[i].classes[idx[i]].class_time_num; k++) {
				for(int p = 0; p < lec[j].classes[idx[j]].class_time_num; p++) {
					if(check_time_overlap(lec[i].classes[idx[i]].times[k].start_time, lec[i].classes[idx[i]].times[k].end_time, lec[j].classes[idx[j]].times[p].start_time, lec[j].classes[idx[j]].times[p].end_time) == 1) {
						flag = 1;
						break;
					}				
				}	
				if(flag == 1) break;
			}
			if(flag == 1) break;
		}
		if(flag == 1) break;
	}	


	if(flag == 0) {
		int rate_sum = 0;
		TIMETABLE add_timetable;
		for(int i = 0; i < lecture_num; i++) {
			rate_sum += lec[i].classes[idx[i]].pref_rate;
			add_timetable.class_numbers.push_back(idx[i]);
		}	
		add_timetable.weight = rate_sum;
		
		timetable.push_back(add_timetable);
	}	

	return 0;
}

int timetable_combination(int cur_lec_num)
{	
	if(cur_lec_num == (lecture_num - 1)) {
		for(int i = 0; i < lec[cur_lec_num].class_num; i++) {
			idx[cur_lec_num] = i;				

			make_timetable();
		}	
	}
	else {
		for(int i = 0; i < lec[cur_lec_num].class_num; i++) {
			idx[cur_lec_num] = i;
			
			timetable_combination(cur_lec_num + 1);
		}			
	}

	return 0;	
}

bool compare(TIMETABLE a, TIMETABLE b)
{
	return a.weight > b.weight;
}

int sort_timetable()
{
	sort(timetable.begin(), timetable.end(), compare);		

	return 0;
}

int main()
{
	FILE *fi, *fo;

	//explanation
	printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n#########################\n");
	printf("TIMETABLE MAKER v1.0\n#########################\n");
	printf("------------------\n");
	printf("Copyright 2021 Doheon Kwon <doheon.kwon@gmail.com>\nALL RIGHTS RESERVED\nREPRODUCTION AND MODIFICATION ONLY WITH AUTHOR'S MARK AND AUTHOR'S PERMISSION\n");
	printf("------------------\n\n");
	printf("Make the 'input.txt'\nThe input file, 'input.txt', must have the following form...\n('_' means the spacebar. Also, you should do the same in input.txt where the line is changed in the form below.)\n\n\n\n\n");
	printf("(the number of lecture)\n(code or name of the lecture)_(the number of class of the lecture)\n(class No.)_(the number of class time)\n(day of the week)_(start time)_(end time)\n(rate of preference)\n\n");
	printf("* day of the week : MON = 1, TUE = 2, WED = 3, THU = 4, FRI = 5\n* time should be written in 24-hour system and you should remove the colon(:) from the time\n* rate of preference should be in the range of 0 to 3\n\n\n");
	printf("input.txt Example :\n\n3\nMAS100 3\nA 2\n1 1300 1430\n3 1300 1430\n2\nB 2\n2 1300 1430\n4 1300 1430\n3\nC 2\n1 1030 1200\n3 1030 1200\n0\nHSS016 1\nA 3\n1 1430 1600\n3 1430 1600\n5 1100 1200\n1\nPH141 1\nA 2\n2 1430 1600\n4 1430 1600\n2\n\n\n");
	printf("------------------\n\n");
	printf("After you made 'input.txt', press 'y' and 'enter' to make your timetable!\n"); 

	char execute_ans;
	cin >> execute_ans;

	
	fi = fopen("input.txt", "r");

	if(fi == NULL) {
		cout << "Can't find input.txt!" << endl;
		return 0;
	}

	printf("\n\non progress...\n\n");

	fscanf(fi, "%d", &lecture_num);

	for(int i = 0; i < lecture_num; i++) {
		LECTURE lec_input;

		fscanf(fi, "%s %d", lec_input.lec_code, &lec_input.class_num);
		for(int j = 0; j < lec_input.class_num; j++) {
			CLASS class_input;
	
			fscanf(fi, "%s %d", class_input.class_name, &class_input.class_time_num);

			for(int k = 0; k < class_input.class_time_num; k++) {
				int day_of_week;
				int st_t;
				int en_t;

				fscanf(fi, "%d %d %d", &day_of_week, &st_t, &en_t); 
				
				TIME time_input;

				time_input.start_time = day_of_week * 10000 + st_t;
				time_input.end_time = day_of_week * 10000 + en_t;

				class_input.times.push_back(time_input);
			}
			fscanf(fi, "%d", &class_input.pref_rate);

			lec_input.classes.push_back(class_input);
		}	

		lec.push_back(lec_input);
	}

	fclose(fi);		

	timetable_combination(0);

	sort_timetable();	

	for(int i = 0; i < timetable.size(); i++) {
		char file_name[100];
		sprintf(file_name, "timetable_%05d.txt", (i + 1));
		fo = fopen(file_name, "w");	

		for(int j = 0; j < lecture_num; j++) {
			fprintf(fo, "%s %s -- preference_rate : %d\n", lec[j].lec_code, lec[j].classes[timetable[i].class_numbers[j]].class_name, lec[j].classes[timetable[i].class_numbers[j]].pref_rate);
			for(int k = 0; k < lec[j].classes[timetable[i].class_numbers[j]].class_time_num; k++) {
				if((lec[j].classes[timetable[i].class_numbers[j]].times[k].start_time / 10000) == 1) {
					char print_content[100];
					sprintf(print_content, "MON %02d:%02d ~ %02d:%02d", (lec[j].classes[timetable[i].class_numbers[j]].times[k].start_time % 10000 / 100), (lec[j].classes[timetable[i].class_numbers[j]].times[k].start_time % 100), (lec[j].classes[timetable[i].class_numbers[j]].times[k].end_time % 10000 / 100), (lec[j].classes[timetable[i].class_numbers[j]].times[k].end_time % 100));

					fprintf(fo, "%s\n", print_content);
				}
				else if((lec[j].classes[timetable[i].class_numbers[j]].times[k].start_time / 10000) == 2) {
					char print_content[100];
					sprintf(print_content, "TUE %02d:%02d ~ %02d:%02d", (lec[j].classes[timetable[i].class_numbers[j]].times[k].start_time % 10000 / 100), (lec[j].classes[timetable[i].class_numbers[j]].times[k].start_time % 100), (lec[j].classes[timetable[i].class_numbers[j]].times[k].end_time % 10000 / 100), (lec[j].classes[timetable[i].class_numbers[j]].times[k].end_time % 100));

					fprintf(fo, "%s\n", print_content);
				}
				else if((lec[j].classes[timetable[i].class_numbers[j]].times[k].start_time / 10000) == 3) {
					char print_content[100];
					sprintf(print_content, "WED %02d:%02d ~ %02d:%02d", (lec[j].classes[timetable[i].class_numbers[j]].times[k].start_time % 10000 / 100), (lec[j].classes[timetable[i].class_numbers[j]].times[k].start_time % 100), (lec[j].classes[timetable[i].class_numbers[j]].times[k].end_time % 10000 / 100), (lec[j].classes[timetable[i].class_numbers[j]].times[k].end_time % 100));

					fprintf(fo, "%s\n", print_content);
				}
				else if((lec[j].classes[timetable[i].class_numbers[j]].times[k].start_time / 10000) == 4) {
					char print_content[100];
					sprintf(print_content, "THU %02d:%02d ~ %02d:%02d", (lec[j].classes[timetable[i].class_numbers[j]].times[k].start_time % 10000 / 100), (lec[j].classes[timetable[i].class_numbers[j]].times[k].start_time % 100), (lec[j].classes[timetable[i].class_numbers[j]].times[k].end_time % 10000 / 100), (lec[j].classes[timetable[i].class_numbers[j]].times[k].end_time % 100));

					fprintf(fo, "%s\n", print_content);
				}
				else if((lec[j].classes[timetable[i].class_numbers[j]].times[k].start_time / 10000) == 5) {
					char print_content[100];
					sprintf(print_content, "FRI %02d:%02d ~ %02d:%02d", (lec[j].classes[timetable[i].class_numbers[j]].times[k].start_time % 10000 / 100), (lec[j].classes[timetable[i].class_numbers[j]].times[k].start_time % 100), (lec[j].classes[timetable[i].class_numbers[j]].times[k].end_time % 10000 / 100), (lec[j].classes[timetable[i].class_numbers[j]].times[k].end_time % 100));

					fprintf(fo, "%s\n", print_content);
				}
			}

			fprintf(fo, "\n\n");
		}

		fclose(fo);
	}		

	printf("Generated all possible timetables! (the files are in the same folder of execution file)\n\n");		
	printf("------------------\n\n");



	return 0;
}
