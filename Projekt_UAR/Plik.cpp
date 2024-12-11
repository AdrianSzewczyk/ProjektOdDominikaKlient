#include "Plik.h"
void Plik::set_sym(Symulator* sym_addr)
{
	sym = sym_addr;
};
void Plik::free_sym() 
{
	sym = nullptr;
};
void Plik::save_config() 
{
	if (sym != nullptr)
	{
		model_ARX* arx_t = sym->get_arx();
		PID_controller* pid_t = sym->get_pid();
		Generator* gen_t = sym->get_gen();
		std::vector<double> write_A = arx_t->get_vector_A();
		std::vector<double> write_B = arx_t->get_vector_B();
		int write_latency = arx_t->get_latency();
		bool write_disuption = arx_t->get_disruption_status();

		double write_P = pid_t->get_k();
		double write_I = pid_t->get_Ti();
		double write_D = pid_t->get_Td();

		double write_Amp = gen_t->get_Amp();
		int write_T = gen_t->get_T();
		double write_fill = gen_t->get_fill();
		std::ofstream file("config.txt");
		if (file.is_open())
		{
			for (double val : write_A)
			{
				file << val << ";";
			}
			file << std::endl;
			for (double val : write_B)
			{
				file << val << ";";
			}
			file << std::endl;
			file << write_latency << std::endl;
			file << write_disuption << std::endl;
			file << write_P << std::endl;
			file << write_I << std::endl;
			file << write_D << std::endl;
			file << write_Amp << std::endl;
			file << write_T << std::endl;
			file << write_fill << std::endl;
			file.close();
		}
	}
};
void Plik::read_config() 
{
	if (sym != nullptr)
	{ 
		std::stringstream ss;
		std::string str;
		std::string temp;
		double val_d;
		std::vector<double> values_read_A;
		std::vector<double> values_read_B;
		int latency_read;
		bool disruption_read;
		double P_read;
		double I_read;
		double D_read;
		double k_read;
		int T_read;
		double fill_read;
		std::ifstream file("config.txt");
		if(file.is_open())
		{
			std::getline(file, str);
			ss << str;
			while(!ss.eof())
			{
				ss >> temp;
				if(std::stringstream(temp)>>val_d)
				{
				values_read_A.push_back(val_d);
				temp = "";
				}
			}
			std::cout << std::endl;
			ss.clear();
			std::getline(file, str);
			ss << str;
			while (!ss.eof())
			{
				ss >> temp;
				if (std::stringstream(temp) >> val_d)
				{
					values_read_B.push_back(val_d);
					temp = "";
				}
			}
			std::getline(file, str);
			latency_read = std::stoi(str);
			str = "";
			std::getline(file, str);
			disruption_read = std::stoi(str);
			sym->set_arx(values_read_A, values_read_B, latency_read, disruption_read);
			std::getline(file, str);
			P_read = std::stod(str);
			std::getline(file, str);
			I_read = std::stod(str);
			std::getline(file, str);
			D_read = std::stod(str);
			sym->set_pid(P_read, I_read, D_read);
			std::getline(file, str);
			k_read = std::stod(str);
			std::getline(file, str);
			T_read = std::stoi(str);
			std::getline(file, str);
			fill_read = std::stod(str);
			sym->set_gen(k_read, T_read, fill_read);
			file.close();
		}
	}
};

void Plik::save_config_bin()
{
	if (sym != nullptr)
	{
		model_ARX* arx_t = sym->get_arx();
		PID_controller* pid_t = sym->get_pid();
		Generator* gen_t = sym->get_gen();
		std::vector<double> write_A = arx_t->get_vector_A();
		std::vector<double> write_B = arx_t->get_vector_B();
		int write_latency = arx_t->get_latency();
		bool write_disuption = arx_t->get_disruption_status();

		double write_P = pid_t->get_k();
		double write_I = pid_t->get_Ti();
		double write_D = pid_t->get_Td();

		double write_Amp = gen_t->get_Amp();
		int write_T = gen_t->get_T();
		double write_fill = gen_t->get_fill();
		
		std::ofstream file("config_bin.txt",std::ios::binary);
		if (file.is_open())
		{
			file.write(reinterpret_cast<*char>(&sym), 1);
			file.close();
		}
	}
};
void Plik::read_config_bin()
{
	if (sym != nullptr)
	{
	double val_d = 0;
	std::vector<double> values_read_A = { 0 };
	std::vector<double> values_read_B = { 0 };
	int latency_read = 0;
	bool disruption_read = 0;
	double P_read = 0;
	double I_read = 0;
	double D_read = 0;
	double k_read = 0;
	int T_read = 0;
	double fill_read = 0;
	int size_A = 0;
	int size_B = 0;
	FILE* file;
	errno_t err;
	if ((err = fopen_s(&file, "config_bin.txt", "rb")) != 0)
	{
		std::cerr << "BLAD";
	}
	else
	{
		rewind(file);

	
		sym->set_arx(values_read_A, values_read_B, latency_read, disruption_read);
		sym->set_pid(P_read, I_read, D_read);
		sym->set_gen(k_read, T_read, fill_read);
		fclose(file);
	}
	}
};