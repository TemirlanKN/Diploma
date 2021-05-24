#include<iostream>
#include<random>
#include<fstream>

using namespace std;
#define ITER 10000
#define REF 1.05

double get_rand(double a, double sigma);
double period_work(int n, int t, double rate, double m, double a, double sigma, double *my_sum, int *pr_sign, double *time);
void input(int argc, char **argv, int *n, int *t, double *total, double *a, double *sigma);
void output(int *n, int *t, double *total, double *a, double *sigma);

int main(int argc, char **argv){
    setlocale(LC_ALL, "rus");
    double m, a, sigma, rate, all_res, total;
	double *my_sum, all_t, time;
    double max_sum, max_rate, max_t, tmp_sum;
    int i, n, t, *pr_sign;

    input(argc, argv, &n, &t, &total, &a, &sigma); 			//input

	if (argc == 2) {
    	output(&n, &t, &total, &a, &sigma); 				//output if argc == 2
	}
	
	my_sum = new double[n];
	pr_sign = new int[n];
	m = total / n / t;

	max_rate = 1.06;
	max_sum = max_t = 0.0;

	for (rate = 1.06; rate <= a + sigma; rate +=0.01) {
		all_res = 0.0;
		all_t = 0.0;
		for (i = 0; i < ITER; i++){
			all_res += period_work(n, t, rate, m, a, sigma, my_sum, pr_sign, &time);
			all_t += time;
		}
		all_t /= ITER;
		all_res /= ITER;
		if (all_res > max_sum) {
			max_sum = all_res;
			max_rate = rate;
			max_t = all_t;
		}
		cout << endl << "||" << rate << " - процент банка" << endl;
		cout << "||Средняя прибыль :" << all_res  << ", средняя жизнь =" << all_t << endl;
	}
	cout << "Максимальная ставка = " << max_rate << ", максимальная выгода = " << max_sum;
	cout << ", период жизни =" << max_t << endl;

	delete [] my_sum;
	delete [] pr_sign;
	return 0;
}

double gen_rand(double a, double sigma){
    random_device rd;
    mt19937 gen(rd());
    normal_distribution<> d(a, sigma);
    return d(gen);
}

double period_work(int n, int t, double rate, double m, double a, double sigma, double *my_sum, int *pr_sign, double *time){
	int i, j;
	double diff_tmp, rand_num, all;
	for (i = 0; i < n; i++){
		pr_sign[i] = 0;
		my_sum[i] = 0.0;
	}
	*time = 0.0;
	all = 0.0;
	for (i = 0; i < t; i++) {
		for (j = 0; j < n; j++) {
			if (pr_sign[j] == 1) {
				continue;
			}
			*time += 1;
			rand_num = gen_rand(a, sigma);
			diff_tmp = (rand_num - rate);
			if (diff_tmp < 0) {
				pr_sign[j] = 1;
				my_sum[j] += m * (rand_num - REF);
			} else {
				my_sum[j] += m * (rate - REF);
			}
		}
	}
	for (i = 0; i < n; i++) {
		all += my_sum[i];
	}
	*time /= n;
	return all;
}

void output(int *n, int *t, double *total, double *a, double *sigma) {
	cout << "|Кол-во предприятий, месяцев, общая сумма денег :" << endl;
    cout << *n << " " << *t << " " << *total << " " << endl;
    cout << "|средняя оплата предприятий по кредиту и дисперсия(например 1.5 = мат ожидание и 0.5 = дисперсия)" << endl;
    cout <<  *a << " " << *sigma << endl;
	return ;
}

void input(int argc, char **argv, int *n, int *t, double *total, double *a, double *sigma) {
    ifstream file;
    ofstream file_out1;
    if (argc == 1){ 
        cout << "|Кол-во предприятий, месяцев, общая сумма денег :" << endl;
        cin >> *n >> *t >> *total;
        cout << "|средний заработок предприятий по кредиту и дисперсия(например 1.5 = мат ожидание и 0.5 = дисперсия):" << endl;
        cin >> *a >> *sigma;
    }   
    else if(argc == 2){ 
        file.open(argv[1]);
        if (!file){
            cout << "Файл не открыт!" << endl;
            return;
        }
        file >> *n >> *t >> *total;
        file >> *a >> *sigma;
        file.close();
    }
    else{
        puts("Error input");
        return;
    }
}
