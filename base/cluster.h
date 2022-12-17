#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include "queue.h"

enum Borders
{
	min_k = 1, // Минимальное число процессоров
	min_tacts = 1, // Минимальное количество тактов работы кластера
	min_queueSize = 5, // Минимальный размер очереди в кластере
	max_k = 64, // Максимальное число процессоров
	max_tacts = 1000, // Максимальное количество тактов работы кластера
	max_queueSize = 50, // Максимальный размер очереди в кластере
	max_dur = 10 // Максимальная длительность выполнения задания
};

struct Statistics
{
	int total_tasks = 0; // Число появившихся заданий
	int failures = 0; // Число отказов (из-за переполнения очереди)
	int completed_tasks = 0; // Число выполненных заданий
	int lost_tasks_in_queue = 0; // Число заданий оставшихся в очереди
	int tasks_in_progress = 0; // Количество выполняемых задач на момент завершения
	int idle_cycles_of_cluster = 0; // Число тактов простоя кластера
	double average_cluster_load = 0.0; // Средняя загрузка кластера

	friend std::ostream& operator<< (std::ostream& out, const Statistics& st); // Вывод статистики в поток вывода
};

struct Task
{
	int id; // Номер задания 
	int time;  // Время начала выполнения - номер такта
	int dur; // Время выполнения задания - длительность в тактах - 1<=d<=10
	int proc; // Количество процессоров, нужных для выполнения задания

	Task(int _id, int _k) : id(_id) // Конструктор, контроль передаваемых параметров за программистом
	{
		time = 0; // Время начала выполнения != времени появления
		dur = rand() % Borders::max_dur + 1;
		proc = rand() % _k + 1;
	}
};

class Cluster
{
private:
	int k; // Количество процессоров, 1<=k<=64
	int tacts; // Количество тактов работы кластера, 1<=T<=1000
	int queueSize; // Размер очереди 5<=size<=50
	double frequency; // Частота появления заданий

public:
	Cluster(int _k, int _tacts, int _queueSize, double _frequency); // Конструктор

	Statistics simulation(); // Моделирование работы
};
