#include "cluster.h"

// Оператор вывода для class Statistics
std::ostream& operator<< (std::ostream& out, const Statistics& st) // Вывод статистики в поток вывода
{
	out << "\tStatistics:\n";
	out << "Number of total tasks: " << st.total_tasks << '\n';
	out << "Number of failures (due to queue overflow): " << st.failures << '\n';
	out << "Number of completed tasks: " << st.completed_tasks << '\n';
	out << "Number of tasks remaining in the queue: " << st.lost_tasks_in_queue << '\n';
	out << "Number of running tasks at the time of completion: " << st.tasks_in_progress << '\n';
	out << "Number of idle cycles of the cluster: " << st.idle_cycles_of_cluster << '\n';
	out << "Average cluster load: " << st.average_cluster_load << '\n';
	return out;
}

// Конструктор для class Cluster
Cluster::Cluster(int _k, int _tacts, int _queueSize, double _frequency) : k(_k), tacts(_tacts), queueSize(_queueSize), frequency(_frequency)
{
	if (k < Borders::min_k || k > Borders::max_k)
		throw std::string("Incorrect number of processors in cluster");

	if (tacts < Borders::min_tacts || tacts > Borders::max_tacts)
		throw std::string("Incorrect maximum number of tacts in the cluster");

	if (queueSize < Borders::min_queueSize || queueSize > Borders::max_queueSize)
		throw std::string("Incorrect maximum queue size in the cluster");

	if (frequency <= 0 || frequency > 1)
		throw std::string("Incorrect frequency in the cluster");

	srand(static_cast<unsigned int>(time(0)));
}

// Симуляция работы Cluster
Statistics Cluster::simulation()
{
	Statistics stat; // Объект класса, в который собирается необходимая статистика

	TQueue<Task> queue; // Очередь для обработки заданий

	std::vector<Task> tasks_in_process; // Вектор, хранящий в себе текущие обрабатываемые задачи

	int current_time = 0; // Номер текущего такта обработки заданий
	int full_cluster_load = 0; // Сумма загрузок кластера за время работы программы

	for (; current_time < tacts; current_time++)
	{
		int current_free_proc = k;

		// Просмотр всех выполняющихся заданий и удаление нужных
		int i = 0;
		while (i < tasks_in_process.size())
		{
			if ((tasks_in_process[i].time + tasks_in_process[i].dur) == current_time)
			{
				tasks_in_process.erase(tasks_in_process.begin() + i); // Завершаем задание путём его удаления
				stat.completed_tasks++;
			}
			else
			{
				current_free_proc -= tasks_in_process[i].proc;
				i++;
			}
		}

		// Появление задания
		if ((double(rand()) / RAND_MAX) <= frequency)
		{
			Task tmp(stat.total_tasks, k);
			if (queue.get_size() < queueSize) // Если очередь не заполнена
				queue.push_back(tmp);
			else
				stat.failures++;
			stat.total_tasks++;
		}

		// Сбор для выполнения заданий из очереди
		while (!queue.empty() && current_free_proc >= queue.get_front().proc)
		{
			Task tmp = queue.pop_front();
			tmp.time = current_time; // Изменяем время начала выполнения на текущий такт
			tasks_in_process.push_back(tmp);
			current_free_proc -= tmp.proc;
		}

		// Если не выполняется ни одна задача
		if (tasks_in_process.size() == 0)
			stat.idle_cycles_of_cluster++;

		full_cluster_load += (k - current_free_proc);
	}
	stat.lost_tasks_in_queue = queue.get_size();
	stat.tasks_in_progress = tasks_in_process.size();
	stat.average_cluster_load = double(full_cluster_load) / (k * tacts);
	return stat;
}
