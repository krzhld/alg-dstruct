#include "stressTest.h"

/*
	CPU:
	Intel(R) Core(TM) i5-1035G1 CPU @ 1.00GHz
	Base speed:	1.19 GHz
	Sockets:	1
	Cores:	4
	Logical processors:	8
	Virtualization:	Enabled
	L1 cache:	320 KB
	L2 cache:	2.0 MB
	L3 cache:	6.0 MB
 
	Memory:
	8.0 GB
	Speed:	2667 MHz
	Slots used:	2 of 2
	Form factor:	SODIMM
	Hardware reserved:	195 MB

	SSD:
	Capacity:	239 GB
	Formatted:	239 GB
 
	Test results:
	Vertex: 40000
	Time: 21s
	Memory: 3.1GB
*/

int main() {
	StressTest();
	return 0;
}
