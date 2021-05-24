#include <iostream>
#include <pthread.h>
#include <random>
#include <fstream>
#include <iomanip>
#include <cmath>
#define count_n 10
using namespace std;
int max_iter = 1000;
double ref = 1.05;

double gen_rand(double a, double sigma);
double work_res(int id, int num_pr, int month, double my_rate, double crdt, double a, double sigma, int pt_count, double *time);
void *period_work_res(void *args);

struct Args{
	int num_pr;
	int month;
	int pt_count;
	int id;
	double crdt;
	double a;
	double sigma;
	double my_rate;
	double *res;
	double *T;
};

int main(int argc, char **argv){
	setlocale(LC_ALL, "rus");
	ifstream file;
	ofstream file_out1;
	ofstream file_out2;
	double crdt, a, sigma, my_rate, bank;
	double all_res, all_T, *T, *res;
	double max, max_rate, max_T, max_sigma;
	double *X, X_tmp, tmp, ans;
	int i, j, num_pr, month, pt_count;
	pthread_t *threads;
	struct Args *args;
	
	if (argc == 1){
		cout << "|Кол-во предприятий, месяцев, общая сумма денег :" << endl;
		cin >> num_pr >> month >> bank;
		cout << "|средний заработок предприятий по кредиту и дисперсия(например 1.5 = мат ожидание и 0.5 = дисперсия):" << endl;
		cin >> a >> sigma;
		cout << "Кол-во процессов :" << endl;
		cin >> pt_count;
	}
	else if(argc == 2 || argc == 4){
		file.open(argv[1]);
		if (!file){
			cout << "Файл не открыт!" << endl;
			return 1;
		}
		file >> num_pr >> month >> bank;
		file >> a >> sigma;
		file >> pt_count;
		cout << "|Кол-во предприятий, месяцев, общая сумма денег :" << endl;
		cout << num_pr << " " << month << " " << bank << " " << endl;
		cout << "|средняя оплата предприятий по кредиту и дисперсия(например 1.5 = мат ожидание и 0.5 = дисперсия)" << endl;
		cout <<  a << " " << sigma << endl;
		//cout << "|Ставка банков 2 уровня ("<< ref << " < например " << (a + ref) / 2 << " < " << a << ")" << endl;
		cout << "Запущено на : " << pt_count << " процессах." << endl;
		file.close();
	}
	else{
		puts("Error input");
		return 0;
	}
	if (argc == 4){
		file_out1.open(argv[2]);
		file_out2.open(argv[3]);
		if (!(file_out1 && file_out2)){
			cout << "Нельзя открыть выходной файл!" << endl;
			return 2;
		}
	}
	
	threads = (pthread_t*)malloc(pt_count * sizeof(pthread_t));
 	args = (struct Args*)malloc(pt_count * sizeof(struct Args));
	res = (double*)malloc(pt_count * sizeof(double));
	T = (double*)malloc(pt_count * sizeof(double));
	X = (double*)malloc(count_n * sizeof(double));
 	if (!(threads && args && res && T)){
		puts("Не достаточно памяти!");
		if (threads) free(threads);
		if (args) free(args);
		if (res) free(res);
		if (T) free(T);
		if (X) free(T);
		return 0;
	}
	crdt = bank / num_pr;
	for (i = 0; i < pt_count; i++){
		args[i].num_pr = num_pr;
		args[i].month = month;
		args[i].pt_count = pt_count;
		args[i].crdt = crdt;
		args[i].a = a;
		args[i].sigma = sigma;
		args[i].id = i;
		args[i].T = T;
		args[i].res = res;
	}
	X_tmp = 0;
	for (j = 0; j < count_n; j++){
		max = 0.0;
		for (my_rate = 1.06; my_rate <= a; my_rate += 0.01){
//			for (a = 1.1; a < 1.5; a += 0.05){
//				for (sigma = 0.01; sigma < 0.5; sigma += 0.05){
						//cout << a << " среднее, " << sigma << " дисперсия.\n";
						all_res = 0;
						all_T = 0;
						for (i = 0; i < pt_count; i++){
							args[i].my_rate = my_rate;
							pthread_create(threads + i, 0, period_work_res, args + i);
						}
						for (i = 0; i < pt_count; i++){
							pthread_join(threads[i], 0);
							all_res += res[i];
							all_T += T[i];
						}
						//cout << endl << "||" << my_rate << " - процент банка" << endl;
						all_res /= max_iter;
						all_T /= max_iter;
						if (all_res > max){
							max = all_res;
							max_rate = my_rate;
							max_T = all_T;
							max_sigma = sigma;
						}
						//cout << "||Средняя прибыль :" << all_res  << ", средняя жизнь =" << all_T << endl;
						if (argc == 4){
							file_out1 << my_rate << "  " << all_res << endl;
							file_out2 << my_rate << "  " << all_T << endl;
						}
//				}
//			}
		}
		cout << "Максимальная ставка = " << max_rate << ", максимальная выгода = " << max;
		//cout << "Максимальная ставка = (" << max_rate << ", " << max_sigma << ") , максимальная выгода = " << max;
		cout << ", период жизни =" << max_T << endl;
		X[j] = max;
		X_tmp += max;
	}
	ans = 0;
	X_tmp /= count_n;
	for (i = 0; i < count_n; i++){
		tmp = X[i] - X_tmp;
		ans += tmp * tmp;
	}
	cout << "М.О. и Дисперсия с.в. выгоды : " << X_tmp << ", " << ans / (count_n - 1) << endl;
	if (argc == 4){
		file_out1.close();
		file_out2.close();
	}
	free(args);
	free(threads);
	free(res);
	free(X);
	return 0;
}

double gen_rand(double a, double sigma){
	random_device rd;
	mt19937 gen(rd());
	normal_distribution<> d(a, sigma);
	return d(gen);
}

void *period_work_res(void *args){
	int i;
	int num_pr, month, id, pt_count;
	double tmp_r = 0, tmp_t = 0;
	double crdt, a, sigma, my_rate;
	double *res, *T, *time;
	struct Args *m = (struct Args*)args;
	time = (double*)malloc(m->num_pr * sizeof(double));
	num_pr = m->num_pr; 
	month = m->month; 
	pt_count = m->pt_count; 
	crdt = m->crdt; 
	a = m->a; 
	sigma = m->sigma;
	my_rate = m->my_rate; 
	id = m->id; 
	res = m->res;
	T = m->T;
	for (i = id; i < max_iter; i += pt_count){
		tmp_r += work_res(id, num_pr, month, my_rate, crdt, a, sigma, pt_count, time);
		tmp_t += time[id];
	}
	res[id] = tmp_r;
	T[id] = tmp_t;
	delete [] time;
}

double work_res(int id, int num_pr, int month, double my_rate, double crdt, double a, double sigma, int pt_count, double *time){
	int i, j, sign;
	double rand_num, *my_sum, *pr_sum, debt, tmp, crdt_month = crdt / month;
	double all, *pr_sgn, *reserve_sum, max_reserve = crdt / 3;
	my_sum = new double [num_pr];
	pr_sgn = new double [num_pr];
	//pr_sum = new double [num_pr];
	//reserve_sum = new double [num_pr];
	for (i = 0; i < num_pr; i++){
		my_sum[i] = 0;
	//	pr_sum[i] = 0;
		pr_sgn[i] = 0;
	//	reserve_sum[i] = 0;
	}
	sign = 0;
	debt = 0;
	time[id] = 0;
	all = 0;
	//my_sum[j]
	for (i = 0; i < month; i++)
		for (j = 0; j < num_pr; j++){
			//cout << endl << "Start, sum :" << pr_sum[j] << endl;
			if (pr_sgn[j] == 1) continue;
			time[id]++;

			//debt = crdt;
			//cout << "time++" << endl;
			//cout << "taken m, sum :" << pr_sum[j] << ", debt :" << debt << endl;
			rand_num = gen_rand(a, sigma);
			//cout << "earned m, sum :" << pr_sum[j] << ", earned :" << rand_num << endl;
			tmp = (rand_num - my_rate);
			if (tmp < 0){
				pr_sgn[j] = 1;
				my_sum[j] += crdt_month * (rand_num - ref);
				continue;
			}
			else{
				my_sum[j] += crdt_month * (my_rate - ref);
			}
			//cout << "after p, sum :" << pr_sum[j] << "+" << reserve_sum[j] << endl << endl;
		}
	for (j = 0; j < num_pr; j++){
	//	cout << my_sum[j] << " ";
		all += my_sum[j];
	}
	//cout << endl;
	time[id] /= num_pr;
	//cout << all << " ";
	delete [] my_sum;
	return all;
}
