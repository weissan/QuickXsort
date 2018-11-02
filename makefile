#
# Copyright (C) 2016 Stefan Edelkamp <edelkamp@tzi.de>
# Copyright (C) 2016 Armin Weiß <armin.weiss@fmi.uni-stuttgart.de>
#
# based on the makefile from http://www.diku.dk/~jyrki/Myris/Kat2014S.html
# Copyright (C) 2014 Amr Elmasry
# Copyright (C) 2014 Jyrki Katajainen
# Copyright (C) 2014 Max Stenmark
#
# Note: move and comparison tests are NOT implemented for all algorithms.
#
#CXX=clang++
CXX=g++
CXXFLAGS=-O3 -std=c++11 -Wall -DNDEBUG -march=native
#CXXFLAGS=-O3 -std=c++1y -Wall -march=native


CXXFLAGS_un=-O3 -std=c++11 -Wall -march=native -funroll-loops -DNDEBUG 
CXXFLAGS_O1=-O1 -std=c++11 -Wall -march=native -DNDEBUG 

header-files:= $(wildcard *.h++)
versions:= $(basename $(header-files))
time-tests:= $(addsuffix .time, $(versions))
comp-tests:= $(addsuffix .comp, $(versions)) 
scan-tests:= $(addsuffix .scan, $(versions)) 
move-tests:= $(addsuffix .move, $(versions)) 
branch-tests:= $(addsuffix .branch, $(versions)) 
cache-tests:= $(addsuffix .cache, $(versions)) 
instruction-tests:= $(addsuffix .count, $(versions))
time-tests-data:= $(addsuffix .datatime, $(versions)) 
comp-tests-data:= $(addsuffix .datacomp, $(versions)) 
move-tests-data:= $(addsuffix .datamove, $(versions)) 
branch-tests-data:= $(addsuffix .databranch, $(versions)) 
cache-tests-data:= $(addsuffix .datacache, $(versions)) 
instruction-tests-data:= $(addsuffix .datacount, $(versions))  
unit-tests:= $(addsuffix .test, $(versions)) 

# N = 512 1024 2048 4096 8192 16384 32768 65536 131072 262144 524288 1048576 2097152 4194304 8388608 16777216 33554432 67108864 134217728 268435456  536870912 1073741824


N = 1024 2048 4096 8192 16384 32768 65536 131072 262144 524288 1048576 2097152 4194304 8388608 16777216 33554432 67108864 134217728 268435456 
#

#N_singlerepetition = 1024 4096 16384 65536 262144 1048576 4194304 16777216 33554432  

N_undersampling = 1048576 4194304 33554432  

#N =  16777216 134217728

many_N = 500 600 720 864 1000 1024 1037 1244 1493 1792 2048 2150 2580 3096 3715 4096 4458 5000 5350 6420 7704 8192 9245 10000 11094 13313 15976 16384 19171 23005 27606 32768 33127 39752 47702 50000 57242 65536 68690 82428 98914 100000 118697 131072 142436 170923 205108 246130 262144 295356 354427 425312 500000

smallN = 1024 2048 4096 8192 16384 32768 65536 131072 262144 524288 1048576 2097152 4194304 8388608 16777216 33554432
N_bc = 1024 2048 4096 8192 16384 32768 65536 131072 262144 524288 1048576 2097152 4194304 8388608 16777216 33554432

verysmallN = 1024 2048 4096 8192 16384 32768 65536 131072 262144 524288 1048576

verylargeN = 33554432 67108864 134217728 268435456 536870912 1073741824 2147483648 4294967296 8589934592 17179869184 34359738368 68719476736 

singlerepetitionQXN = 131072 196608 262144 393216 524288 786432 1048576

denseN = 1048576 1123836 1204498 1290948 1383604 1482910 1589344 1703417 1825677 1956712 2097152 2247672 2408995 2581897 2767209 2965821 3178688 3406833 3651354 3913424 4194304 

smalldenseN = 262144 280959 301124 322737 345901 370728 397336 425854 456419 489178 524288 561918 602249 645474 691802 741455 794672 851708 912838 978356 1048576 

N_few =  1024 16384 131072 1048576 16777216 134217728

fewseeds = 1419836757 1736079990 3049352726 138745579 3507014995 1211763785 2069625985 1791738544 901852307 3707813544 

seeds = 1419836757 1736079990 3049352726 138745579 3507014995 1211763785 2069625985 1791738544 901852307 3707813544 2738501551 1359104479 4090736199 338537062 198114031 1793305874 932609687 109094632 719137151 3048484656 3784540045 1638271900 1974537596 4221393117 1375474640 1543833964 3478869777 2752787099 2507895769 3874234565 2051206898 3274687509 1186671856 2505935960 255060488 4226744669 2444449807 3358769604 1635000407 3920146827

hundredseeds = 1419836757 1736079990 3049352726 138745579 3507014995 1211763785 2069625985 1791738544 901852307 3707813544 2738501551 1359104479 4090736199 338537062 198114031 1793305874 932609687 109094632 719137151 3048484656 3784540045 1638271900 1974537596 4221393117 1375474640 1543833964 3478869777 2752787099 2507895769 3874234565 2051206898 3274687509 1186671856 2505935960 255060488 4226744669 2444449807 3358769604 1635000407 3920146827 2571551238 1126363349 178227716 3477740041 2219924689 2981471014 4276900108 30479107 1949351857 1460248837 53014025 165225233 3416447845 890408661 300320738 2856513422 1893322240 1432585755 4062341515 3097406414 1798884608 1330510730 3899149452 1933692990 3705262070 1218249789 3848001977 4133388893 2238990247 499925814 561225270 3929456888 3438171099 3468296470 749452073 2617798902 1601935687 318690207 524356049 4252437271 2470077673 226799234 1073427801 2958598886 1372317703 254594514 613851155 2354859646 2762502478 37924386 1642692745 3964049201 2471987473 36737599 3146196209 2137029703 943854286 1333681903 313323716 520777759 
 
moreseeds = 1419836757 1736079990 3049352726 138745579 3507014995 1211763785 2069625985 1791738544 901852307 3707813544 2738501551 1359104479 4090736199 338537062 198114031 1793305874 932609687 109094632 719137151 3048484656 3784540045 1638271900 1974537596 4221393117 1375474640 1543833964 3478869777 2752787099 2507895769 3874234565 2051206898 3274687509 1186671856 2505935960 255060488 4226744669 2444449807 3358769604 1635000407 3920146827 2571551238 1126363349 178227716 3477740041 2219924689 2981471014 4276900108 30479107 1949351857 1460248837 53014025 165225233 3416447845 890408661 300320738 2856513422 1893322240 1432585755 4062341515 3097406414 1798884608 1330510730 3899149452 1933692990 3705262070 1218249789 3848001977 4133388893 2238990247 499925814 561225270 3929456888 3438171099 3468296470 749452073 2617798902 1601935687 318690207 524356049 4252437271 2470077673 226799234 1073427801 2958598886 1372317703 254594514 613851155 2354859646 2762502478 37924386 1642692745 3964049201 2471987473 36737599 3146196209 2137029703 943854286 1333681903 313323716 520777759 1690331145 74769077 2754618751 3327953111 3880417323 2536838055 4114756985 4189641205 2182629803 570401083 2737126284 777402150 3117089758 1140788806 3234707986 3041153712 1407683364 2514741446 920512742 3419272804 977122057 2344577273 1709684483 3147676661 2220072875 226305867 330292701 1226695439 4155365385 1307539223 2814704356 3085225970 1196352270 928383066 3782058722 381155440 1276568734 659381334 331400134 1099894884 4136684040 2734906598 1734452946 854650022 3740339598 3643526748 3056820912 731591042 2017694048 1366578448 1968712304 1905542317 1337874240 2489024182 2535724944 980238589 1573416222 2030497247 3876170531 1671581369 3289759456 3531355868 380559316 2485155584 4128885507 271562901 473562292 24658192 1901932194 170100073 988968155 428969788 4257625747 3400413072 84734211 226318962 2573069420 3121154462 1814974665 154992079 3352621950 2255284527 2158324604 60609687 1744870686 647633271 1063564738 4261194623 2082696479 985354850 1773609836 4230865129 1829338238 1976852815 2727254630 1629908571 532562514 3776025057 2678679040 1591519997 3242180117 1480833309 2675636148 4044090000 204778127 825820316 1682536619 1630693281 518090347 52724711 4000411605 978319228 3711123451 218155617 4118391985 1418935835 568293351 4128742495 1373848541 2679309429 3416898521 1269687279 1607423878 3856725102 1012452328 1180232155 1448527175 2574373096 2175011869 310604288 1715690004 3188385118 833615168 2152480575 4197636902 3877745090 2094564331 3010445831 1287823937 3577446060 2226683374 801577188 4130787354 3138618086 1879999237 2643083265 3939459262 3989431260 1623826817 1127969100 4015549287 4129196198 3088729617 561687736 2071788716 2540382703 1568338424 1108947642 3739859905 2776191307 1478883604 1011854229 3581639854 1619316695 3412147078 904573300 3535481991 3822331810 1929053667 191134137 253861296 3623365417 3133420531 3166318005 447115311 2316499043 1301894331 19865354 498189299 3956391824 2175170329 4227924889 781811863 2667347851 1082235571 4260722181 326486029 2068511423 2477636358 4049015665 1458309631 3682951390 2192073365 4055524890 2868549507 1863341104 668697790 1060004710 42928226 272071972 1469537861 4194633192 2625205673 591662840 1590213155 669483990 2713213023 878848978 2282913343 716374726 2973234430 1459200399 4244578110 2713963261 958565248 1210301077 18520832 1531269017 4277697712 272474917 3278602577 2853293560 2287055993 3988586562 3207633583 4232441414 1459339023 224983896 4281934729 1373993966 3208359266 3638523687 1915692794 1790402397 411993564 1404380385 4207291558 1681331893 1841760796 1683043761 3506458828 2264324193 3343249156 3174166511 3173065934 384241427 2797330176 3679334439 492366848 3098267299 2405359674 3894855358 3286968792 3124957061 2842274422 797523370 2568048306 2225863034 3681299221 1847531842 207002032 3719915523 216892978 3575801653 288005023 156168072 504241198 2199681200 4261310410 629961400 2952616225 1465625176 396521319 3226063307 4202201445 49584603 2415661164 2947036309 1642069415 4280909828 3184818106 2088726386 2017454566 4191510248 1602049654 506057942 903235112 4102110700 1924013859 827917933 1615691022 29379922 2657135630 1822545510 2575157828 1479057472 382957118 3627672564 2531572250 1445660510 
  
twenty = 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20

hundred  = 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20
  

algorithms = stl stl_gcc heapsort in_situ_merge quickmerge quickmerge_larger_merge quickmerge_worst_case_efficient quickmerge_worst_case_efficient_3_5 quickmerge_hybrid 


allalgorithms =  stl_stable_sort stl stl_gcc_new stl_partial_sort in_situ_merge quickmerge quickmerge_larger_merge quickmerge_worst_case_efficient quickmerge_worst_case_efficient_3_5 quickmerge_hybrid quickmerge_smaller_merge

#qmsalgorithms =  stl_stable_sort stl stl_gcc heapsort in_situ_merge quickmerge quickmerge_larger_merge quickmerge_worst_case_efficient quickmerge_worst_case_efficient_3_5 quickmerge_hybrid quickmerge_hybrid_mo3 

#qmsalgorithms =  stl_stable_sort stl stl_gcc stl_gcc_qms_intro stl_gcc_new stl_gcc_new_qms_intro heapsort in_situ_merge quickmerge quickmerge_larger_merge quickmerge_worst_case_efficient quickmerge_worst_case_efficient_3_5 quickmerge_worst_case_efficient_3_5_sqrt_hybrid  quickmerge_worst_case_efficient_3_5_sqrt_hybrid_WORST_CASE quickmerge_hybrid quickmerge_hybrid_mo3 quickmerge_worst_case_efficient_3_5_WORST_CASE quickmerge_worst_case_efficient_WORST_CASE

qmsalgorithms =  stl_stable_sort stl_gcc_new in_situ_merge quickmerge quickmerge_larger_merge quickmerge_worst_case_efficient_3_5 quickmerge_hybrid_mo3 mergesort wikisort

qXalgs = stl_stable_sort stl_gcc_new in_situ_merge quickmerge_smaller_merge quickmerge_larger_merge quickmerge_Reinhardt_mo3 quickmerge_mosqrt quickmerge_mosqrt_small_base quickmerge_no_sampling_larger_merge quickmerge_binaryinsertionsort_mosqrt quickmerge_mergeinsertion_mosqrt mergesort quickheap wikisort

qXalgsComp = stl_stable_sort quickmerge_smaller_merge quickmerge_larger_merge quickmerge_Reinhardt_mo3 quickmerge_mosqrt quickmerge_mosqrt_small_base quickmerge_no_sampling_larger_merge quickmerge_binaryinsertionsort_mosqrt quickmerge_mergeinsertion_mosqrt mergesort

qXalgssmall = stl_stable_sort stl_gcc_new quickmerge_smaller_merge quickmerge_larger_merge quickmerge_Reinhardt_mo3 quickmerge_mosqrt quickmerge_mosqrt_small_base quickmerge_no_sampling_larger_merge mergesort  

singlerepetitionQXAlgs = quickmerge_no_sampling_larger_merge quickmerge_smaller_merge quickmerge_Reinhardt_mo3 quickmerge_larger_merge stl_gcc_new_pivot stl_gcc_new_no_is quickmerge_mosqrt

mqmsalgorithms =  stl_stable_sort stl_gcc_new in_situ_merge quickmerge_worst_case_efficient_3_5_undersampling quickmerge_worst_case_efficient_3_5_undersampling_WORST_CASE_SKEW quickmerge_worst_case_efficient_3_5_undersampling_WORST_CASE quickmerge_hybrid_mo3 wikisort stl_partial_sort 

mqmsOnlyalgorithms = quickmerge_worst_case_efficient_3_5_WORST_CASE quickmerge_worst_case_efficient_3_5_undersampling_WORST_CASE_SKEW quickmerge_worst_case_efficient_3_5_undersampling_WORST_CASE quickmerge_worst_case_efficient_WORST_CASE quickmerge_worst_case_efficient quickmerge_worst_case_efficient_3_5 quickmerge_worst_case_efficient_3_5_undersampling

mqmsFewalgorithms = quickmerge_worst_case_efficient_3_5_undersampling quickmerge_worst_case_efficient_3_5 quickmerge_worst_case_efficient_3_5_undersampling_WORST_CASE_SKEW

mqmsHybridalgorithms = stl_gcc_new stl_gcc_new_qms_intro quickmerge_hybrid_mo3

qms_wc_skew_test_alg =  quickmerge_worst_case_efficient_3_5_undersampling_WORST_CASE_SKEW quickmerge_worst_case_efficient_3_5_undersampling_WORST_CASE

insertionSortSizes = 4 5 6 7 8 9 10 11 12 13 14 15 16 17 118 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39 40 42 44 46 48 50 55 60 70 80 90 100

insertionSortAlgs = quickmerge

undersampling_sizes = 47 48 49 50 51 52 53 54 55 56 57 58 59 60 61 62 63 64 65 66 67 68 69 70 71 72 73 74 75 76 77 78 79 80 81 82 83 84

insSortN = 16384 16777216

fewcomptestAlgs = quickmerge stl_gcc_new stl_gcc stl_gcc_new_random stl_gcc_new_random_pivot

data = b c d i m o p q r s t u v w z
smalldata = i m w
compdata = r m i
mqmsHybriddata = r a h

mqmstypes = int Record Logint PointerRecord
type =  int Record PointerRecord Logint Vector 
qXtypes =  int Logint PointerRecord
# double Rational    double Rational 


unit: 	$(unit-tests) 
 
all: 	$(time-tests) $(comp-tests) $(move-tests) 

data: 	$(move-tests-data)  $(comp-tests-data) $(time-tests-data) 



timetest:
	@for seed in $(seeds) ; do \
		for alg in $(algorithms); do \
			echo $$alg ; \
			cp $$alg.h++ algorithm.h++ ; \
			for t in $(type) ; do \
				echo $$t ; \
				$(CXX) $(CXXFLAGS) -DNAME=$$alg -DTYPE=$$t driver.cpp ; \
				for n in $(N) ; do \
					./a.out $$n r $$seed >> results.time.csv; \
				done ; \
				rm -f ./a.out ; \
			done ; \
			$(CXX) $(CXXFLAGS) -DNAME=$$alg -DTYPE=int driver.cpp ; \
				for d in $(smalldata) ; do \
				  echo $$d ; \
				  for n in $(smallN) ; do \
					./a.out $$n $$d $$seed >> results.time.csv; \
				  done ; \
				done; \
			rm -f ./a.out ; \
			$(CXX) $(CXXFLAGS_un) -DNAME=$$alg -DTYPE=int driver.cpp ; \
				  for n in $(smallN) ; do \
					./a.out $$n r $$seed >> results.unroll.time.csv; \
				  done ; \
			rm -f ./a.out ; \
			$(CXX) $(CXXFLAGS_O1) -DNAME=$$alg -DTYPE=int driver.cpp ; \
				  for n in $(smallN) ; do \
					./a.out $$n r $$seed >> results.O1.time.csv; \
				  done ; \
			rm -f ./a.out ; \
		done ;\
	done
	

singlerepetitioncomptest:
	@echo "algorithm,size,comp,per_n,per_nlogn,kappa" >> results.singlerepetitioncomptest.qX.csv; \
	for seed in $(moreseeds) ; do \
		for alg in $(singlerepetitionQXAlgs); do \
			echo $$alg ; \
			cp $$alg.h++ algorithm.h++ ; \
			$(CXX) $(CXXFLAGS) -DNAME=$$alg -DMEASURE_COMPARISONS -DREPETITIONS=1 -DTOFILE driver.cpp ; \
			for x in $(hundred) ; do \
				for n in $(verysmallN) ; do \
					./a.out $$n r >> results.singlerepetitioncomptest.qX.csv; \
				done ; \
			done ;\
			rm -f ./a.out ; \
		done ;\
	done
	

	
singlerepetitionscantest:
	@echo "algorithm,size,scan,per_n,per_nlogn" >> results.singlerepetitionscantest.csv; \
	for seed in $(seeds) ; do \
		for alg in $(fewcomptestAlgs); do \
			echo $$alg ; \
			cp $$alg.h++ algorithm.h++ ; \
			$(CXX) $(CXXFLAGS) -DNAME=$$alg -DMEASURE_SCANS -DREPETITIONS=1 -DTOFILE driver.cpp ; \
			for x in $(twenty) ; do \
				for n in $(smallN) ; do \
					./a.out $$n r >> results.singlerepetitionscantest.csv; \
				done ; \
			done ;\
			rm -f ./a.out ; \
		done ;\
	done
	
singlerepetitiontimetest:
	@echo "algorithm,mode,parameter,type,size,time,partial_sort_count,repetitions" >> results.singlerepetitiontimetest.qX.csv; \
	for seed in $(moreseeds) ; do \
		for alg in $(singlerepetitionQXAlgs); do \
			echo $$alg ; \
			cp $$alg.h++ algorithm.h++ ; \
			$(CXX) $(CXXFLAGS) -DNAME=$$alg -DREPETITIONS=1 -DTYPE=int -DTOFILE driver.cpp ; \
			for x in $(hundred) ; do \
				for n in $(singlerepetitionQXN) ; do \
					./a.out $$n r >> results.singlerepetitiontimetest.qX.csv; \
				done ; \
			done ;\
			rm -f ./a.out ; \
		done ;\
	done
	
	
	
	

mqmsAlgtimetest:
	@rm -f results.mqmsAlg.time.csv ; \
	echo "algorithm,mode,parameter,type,size,time,partial_sort_count,repetitions" >> results.mqmsAlg.time.csv; \
	for seed in $(hundredseeds) ; do \
		for alg in $(mqmsalgorithms); do \
			echo $$alg ; \
			cp $$alg.h++ algorithm.h++ ; \
			$(CXX) $(CXXFLAGS) -DNAME=$$alg -DTOFILE driver.cpp ; \
			for n in $(N) ; do \
				./a.out $$n r $$seed >> results.mqmsAlg.time.csv; \
			done ; \
			rm -f ./a.out ; \
		done ;\
		sed -i -e 's/ //g' results.mqmsAlg.time.csv ;\
	done
	
mqmsOnlytimetest:
	@rm -f results.mqmsOnly.time.csv ; \
	echo "algorithm,mode,parameter,type,size,time,partial_sort_count,repetitions" >> results.mqmsOnly.time.csv; \
	for seed in $(hundredseeds) ; do \
		for alg in $(mqmsOnlyalgorithms); do \
			echo $$alg ; \
			cp $$alg.h++ algorithm.h++ ; \
			$(CXX) $(CXXFLAGS) -DNAME=$$alg -DTOFILE driver.cpp ; \
			for n in $(N) ; do \
				./a.out $$n r $$seed >> results.mqmsOnly.time.csv; \
			done ; \
			rm -f ./a.out ; \
		done ;\
		sed -i -e 's/ //g' results.mqmsOnly.time.csv ;\
	done
	
	
undersamplingcomptest:
	@rm -f results.undersamplingcomptest.csv ; \
	echo "algorithm,size,undersampling_size,comp,per_n,per_nlogn,kappa" >> results.undersamplingcomptest.csv; \
	for alg in $(qms_wc_skew_test_alg); do \
		cp $$alg.h++ algorithm.h++ ; \
		for x in $(undersampling_sizes) ; do \
			$(CXX) $(CXXFLAGS) -DNAME=$$alg -DMEASURE_COMPARISONS -DUNDERSAMPLING_TEST -DUNDERSAMPLING_SIZE=$$x -DREPETITIONS=1 -DTOFILE driver.cpp ; \
			for seed in $(hundredseeds) ; do \
				for n in $(N_undersampling) ; do \
					./a.out $$n r $$seed >> results.undersamplingcomptest.csv; \
				done ; \
			done ; \
			rm -f ./a.out ; \
		done ; \
		sed -i -e 's/ //g' results.undersamplingcomptest.csv ;\
	done	
		
	
mqmsOnlycomptest:
	@rm -f results.mqmsOnly.comp.csv ; \
	echo "algorithm,size,comp,per_n,per_nlogn,kappa" >> results.mqmsOnly.comp.csv; \
	for seed in $(hundredseeds) ; do \
		for alg in $(mqmsOnlyalgorithms); do \
			echo $$alg ; \
			cp $$alg.h++ algorithm.h++ ; \
			$(CXX) $(CXXFLAGS) -DNAME=$$alg -DMEASURE_COMPARISONS -DTOFILE driver.cpp ; \
			for n in $(N) ; do \
				./a.out $$n r $$seed >> results.mqmsOnly.comp.csv; \
			done ; \
			rm -f ./a.out ; \
		done ;\
		sed -i -e 's/ //g' results.mqmsOnly.comp.csv ;\
	done
	
mqmsHybridtimetest:
	@rm -f results.mqmsHybrid.time.csv ; \
	echo "algorithm,mode,parameter,type,size,time,partial_sort_count,repetitions" >> results.mqmsHybrid.time.csv; \
	for seed in $(hundredseeds) ; do \
		for alg in $(mqmsHybridalgorithms); do \
			echo $$alg ; \
			cp $$alg.h++ algorithm.h++ ; \
			$(CXX) $(CXXFLAGS) -DNAME=$$alg -DTOFILE driver.cpp ; \
			for d in $(mqmsHybriddata) ; do \
			echo $$d ; \
				for n in $(N) ; do \
					./a.out $$n $$d $$seed >> results.mqmsHybrid.time.csv; \
				done ; \
			done ; \
			rm -f ./a.out ; \
		done ;\
		sed -i -e 's/ //g' results.mqmsHybrid.time.csv ;\
	done


mqmsFewtimecomptestManyN:
	@echo "algorithm,mode,parameter,type,size,time,partial_sort_count,repetitions" >> results.mqmsFew.time.csv; \
	echo "algorithm,size,comp,per_n,per_nlogn,kappa" >> results.mqmsFew.comp.csv; \
	for seed in $(hundredseeds) ; do \
		for alg in $(mqmsFewalgorithms); do \
			echo $$alg ; \
			cp $$alg.h++ algorithm.h++ ; \
			$(CXX) $(CXXFLAGS) -DNAME=$$alg -DTOFILE driver.cpp ; \
			for n in $(many_N) ; do \
				./a.out $$n r $$seed >> results.mqmsFew.time.csv; \
			done ; \
			rm -f ./a.out ; \
			$(CXX) $(CXXFLAGS) -DNAME=$$alg -DMEASURE_COMPARISONS -DTOFILE driver.cpp ; \
			for n in $(many_N) ; do \
				./a.out $$n r $$seed >> results.mqmsFew.comp.csv; \
			done ; \
			rm -f ./a.out ; \
		done ;\
		sed -i -e 's/ //g' results.mqmsFew.time.csv ;\
		sed -i -e 's/ //g' results.mqmsFew.comp.csv ;\
	done		

	
	
mqmsAlgtimetesttypes:
	@rm -f results.mqmsAlg.types.time.csv ; \
	echo "algorithm,mode,parameter,type,size,time,partial_sort_count,repetitions" >> results.mqmsAlg.types.time.csv; \
	for seed in $(hundredseeds) ; do \
		for alg in $(mqmsalgorithms); do \
			echo $$alg ; \
			cp $$alg.h++ algorithm.h++ ; \
			for t in $(mqmstypes) ; do \
				echo $$t ; \
				$(CXX) $(CXXFLAGS) -DNAME=$$alg -DTYPE=$$t -DTOFILE driver.cpp ; \
				for n in $(N) ; do \
					./a.out $$n r $$seed >> results.mqmsAlg.types.time.csv; \
				done ; \
			done ; \
			rm -f ./a.out ; \
			sed -i -e 's/ //g' results.mqmsAlg.types.time.csv ;\
		done ;\
	done
	
allAlgtimetest:
	@for seed in $(seeds) ; do \
		for alg in $(allalgorithms); do \
			echo $$alg ; \
			cp $$alg.h++ algorithm.h++ ; \
			for t in $(type) ; do \
				echo $$t ; \
				$(CXX) $(CXXFLAGS) -DNAME=$$alg -DTYPE=$$t -DTOFILE driver.cpp ; \
				for n in $(N) ; do \
					./a.out $$n r $$seed >> results.allAlg.time.csv; \
					./a.out $$n m $$seed >> results.allAlg.time.csv; \
				done ; \
			done ; \
			rm -f ./a.out ; \
		done ;\
	done
	
	
qmsInsertionSorttest:
	@for seed in $(seeds) ; do \
		for alg in $(insertionSortAlgs); do \
			echo $$alg ; \
			cp $$alg.h++ algorithm.h++ ; \
			for t in $(type) ; do \
				echo $$t ; \
				for d in $(insertionSortSizes) ; do \
				echo $$t ; \
					$(CXX) $(CXXFLAGS) -DNAME=$$alg -DTYPE=$$t -DTOFILE -DINS_SIZE_TEST -DINS_SIZE=$$d driver.cpp ; \
					for n in $(insSortN) ; do \
						./a.out $$n r $$seed >> results.qmsInsertionSorttest.time.csv; \
					done ; \
				done ; \
			done ; \
			rm -f ./a.out ; \
		done ;\
	done
	
	
qXcomptest:
	@rm -f results.qX.comp.csv ; \
	echo "algorithm,size,comp,per_n,per_nlogn,kappa" >> results.qX.comp.csv; \
	for seed in $(moreseeds) ; do \
		for alg in $(qXalgsComp); do \
			echo $$alg ; \
			cp $$alg.h++ algorithm.h++ ; \
			$(CXX) $(CXXFLAGS) -DNAME=$$alg -DMEASURE_COMPARISONS -DTOFILE driver.cpp ; \
			for n in $(N) ; do \
				./a.out $$n r $$seed >> results.qX.comp.csv; \
			done ; \
			rm -f ./a.out ; \
		done ;\
		sed -i -e 's/ //g' results.qX.comp.csv ;\
	done
	
qXcomptestdense:
	@rm -f results.qX.comp.dense.csv ; \
	echo "algorithm,size,comp,per_n,per_nlogn,kappa" >> results.qX.comp.dense.csv; \
	for seed in $(moreseeds) ; do \
		for alg in $(qXalgsComp); do \
			echo $$alg ; \
			cp $$alg.h++ algorithm.h++ ; \
			$(CXX) $(CXXFLAGS) -DNAME=$$alg -DMEASURE_COMPARISONS -DTOFILE driver.cpp ; \
			for n in $(denseN) ; do \
				./a.out $$n r $$seed >> results.qX.comp.dense.csv; \
			done ; \
			rm -f ./a.out ; \
		done ;\
		sed -i -e 's/ //g' results.qX.comp.dense.csv ;\
	done
	
qXcomptestsmalldense:
	@rm -f results.qX.comp.smalldense.csv ; \
	echo "algorithm,size,comp,per_n,per_nlogn,kappa" >> results.qX.comp.smalldense.csv; \
	for seed in $(moreseeds) ; do \
		for alg in $(qXalgsComp); do \
			echo $$alg ; \
			cp $$alg.h++ algorithm.h++ ; \
			$(CXX) $(CXXFLAGS) -DNAME=$$alg -DMEASURE_COMPARISONS -DTOFILE driver.cpp ; \
			for n in $(smalldenseN) ; do \
				./a.out $$n r $$seed >> results.qX.comp.smalldense.csv; \
			done ; \
			rm -f ./a.out ; \
		done ;\
		sed -i -e 's/ //g' results.qX.comp.smalldense.csv ;\
	done

qXtimetest:
	@rm -f results.qX.time.csv ; \
	echo "algorithm,mode,parameter,type,size,time,partial_sort_count,repetitions" >> results.qX.time.csv; \
	for seed in $(hundredseeds) ; do \
		for alg in $(qXalgs); do \
			echo $$alg ; \
			cp $$alg.h++ algorithm.h++ ; \
			for t in $(qXtypes) ; do \
				$(CXX) $(CXXFLAGS) -DNAME=$$alg -DTYPE=$$t -DTOFILE driver.cpp ; \
				for n in $(N) ; do \
					./a.out $$n r $$seed >> results.qX.time.csv; \
				done ; \
			done ; \
			rm -f ./a.out ; \
		done ;\
		sed -i -e 's/ //g' results.qX.time.csv ;\
	done
	
qXtimetestverylarge:
	@rm -f results.qX.time.verylarge.csv ; \
	echo "algorithm,mode,parameter,type,size,time,partial_sort_count,repetitions" >> results.qX.time.verylarge.csv; \
	for seed in $(fewseeds) ; do \
		for alg in $(qXalgs); do \
			echo $$alg ; \
			cp $$alg.h++ algorithm.h++ ; \
			for t in $(qXtypes) ; do \
				$(CXX) $(CXXFLAGS) -DNAME=$$alg -DTYPE=$$t -DTOFILE driver.cpp ; \
				for n in $(verylargeN) ; do \
					./a.out $$n r $$seed >> results.qX.time.verylarge.csv; \
				done ; \
			done ; \
			rm -f ./a.out ; \
		done ;\
		sed -i -e 's/ //g' results.qX.time.verylarge.csv ;\
	done
	
qXtimetestsmall:
	@rm -f results.qX.time.int.csv ; \
	echo "algorithm,mode,parameter,type,size,time,partial_sort_count,repetitions" >> results.qX.time.int.csv; \
	for seed in $(hundredseeds) ; do \
		for alg in $(qXalgs); do \
			echo $$alg ; \
			cp $$alg.h++ algorithm.h++ ; \
			$(CXX) $(CXXFLAGS) -DNAME=$$alg -DTYPE=int -DTOFILE driver.cpp ; \
			for n in $(N) ; do \
				./a.out $$n r $$seed >> results.qX.time.int.csv; \
			done ; \
			rm -f ./a.out ; \
		done ;\
		sed -i -e 's/ //g' results.qX.time.int.csv ;\
	done
	
	
qmsAlgtimetestW:
	@echo "algorithm,mode,parameter,type,size,time,partial_sort_count,repetitions" >> results.qmsAlgN.time.csv; \
	for seed in $(seeds) ; do \
		for alg in $(mqmsalgorithms); do \
			echo $$alg ; \
			cp $$alg.h++ algorithm.h++ ; \
			for t in $(type) ; do \
				echo $$t ; \
				$(CXX) $(CXXFLAGS) -DNAME=$$alg -DTYPE=$$t -DTOFILE driver.cpp ; \
				for n in $(N) ; do \
					./a.out $$n r $$seed >> results.qmsAlgN.time.csv; \
				done ; \
			done ; \
			rm -f ./a.out ; \
		done ;\
	done



	 
pivotmethodtest:
	@for seed in $(seeds) ; do \
		for t in $(type) ; do \
			for alg in $(pivotTestAlgs); do \
				cp $$alg.h++ algorithm.h++ ; \
				$(CXX) $(CXXFLAGS) -DNAME=$$alg -DTYPE=$$t driver.cpp ; \
				for n in $(N) ; do \
					./a.out $$n r $$seed >> results.pivotmethod.time.csv; \
				 done ;\
			done ;\
		done ;\
	 done
	 
pivotmethodcomptest:
	@for seed in $(seeds) ; do \
		for alg in $(pivotTestAlgs); do \
			cp $$alg.h++ algorithm.h++ ; \
			$(CXX) $(CXXFLAGS) -DNAME=$$alg -DMEASURE_COMPARISONS driver.cpp ; \
			for n in $(N) ; do \
				./a.out $$n r $$seed >> results.pivotmethod.comp.csv; \
			 done ;\
		done ;\
	 done
	 
pivotmethodmovetest:
	@for seed in $(seeds) ; do \
		for alg in $(pivotTestAlgs); do \
			cp $$alg.h++ algorithm.h++ ; \
			$(CXX) $(CXXFLAGS) -DNAME=$$alg -DMEASURE_MOVES driver.cpp ; \
			for n in $(N) ; do \
				./a.out $$n r $$seed >> results.pivotmethod.move.csv; \
			 done ;\
		done ;\
	 done
	 

	
#-DTOFILE

$(time-tests): %.time : %.h++
	@cp $*.h++ algorithm.h++ 
	@for t in $(type) ; do \
		echo $$t ; \
		$(CXX) $(CXXFLAGS) -DNAME=$*  -DTYPE=$$t driver.cpp ; \
		for n in $(N) ; do \
			./a.out $$n r ; \
		done ; \
		rm -f ./a.out ; \
	done
	
#-DINDEXSORT
	


$(move-tests): %.move : %.h++
	@cp $*.h++ algorithm.h++
	$(CXX) $(CXXFLAGS) -DMEASURE_MOVES -DNAME=$* driver.cpp
	@for d in $(compdata) ; do \
	  echo $$d ; \
	  for n in $(N) ; do \
	    ./a.out $$n $$d ; \
	  done \
	done; \
	rm -f ./a.out 

$(comp-tests): %.comp : %.h++
	@cp $*.h++ algorithm.h++
	$(CXX) $(CXXFLAGS) -DMEASURE_COMPARISONS -DNAME=$* driver.cpp
	@for d in $(compdata) ; do \
	  echo $$d ; \
	  for n in $(N) ; do \
	    ./a.out $$n $$d ; \
	  done \
	done; \
	rm -f ./a.out 
	



$(time-tests-data): %.datatime : %.h++
	@cp $*.h++ algorithm.h++ 
	@for seed in $(seeds) ; do \
		for t in $(type) ; do \
			echo $$t ; \
			$(CXX) $(CXXFLAGS) -DNAME=$* -DTYPE=$$t driver.cpp ; \
			for d in $(data) ; do \
			  echo $$d ; \
			  for n in $(N) ; do \
				./a.out $$n $$d $$seed >> results.time.csv; \
			  done ; \
			done; \
			rm -f ./a.out ; \
		done ;\
	done


$(move-tests-data): %.datamove : %.h++
	@cp $*.h++ algorithm.h++
	$(CXX) $(CXXFLAGS) -DMEASURE_MOVES -DNAME=$* driver.cpp
	@for d in $(data) ; do \
	  echo $$d ; \
	  for n in $(N) ; do \
	    ./a.out $$n $$d >> $*.$$d.move.csv; \
	  done \
	done ; \
	rm -f algorithm.h++ ./a.out 

$(comp-tests-data): %.datacomp : %.h++
	@cp $*.h++ algorithm.h++
	$(CXX) $(CXXFLAGS) -DMEASURE_COMPARISONS -DNAME=$* driver.cpp
	@for d in $(data) ; do \
	  echo $$d ; \
	  for n in $(N) ; do \
	    ./a.out $$n $$d >> $*.$$d.comp.csv; \
	  done \
	done ; \
	rm -f algorithm.h++ ./a.out 


TESTFLAGS=-O3 -std=c++11 -Wall -x c++ -g -DDEBUG 

$(unit-tests): %.test : %.h++
	@cp $*.h++ algorithm.h++
	$(CXX) $(TESTFLAGS) -DNAME=$* driver.cpp
	./a.out
	rm -f algorithm.h++ ./a.out 
	


# Other tools

clean:
	- rm  -f a.out temp algorithm.h++ *.csv 2>/dev/null

veryclean: clean
	- rm -f *~ */*~ 2>/dev/null

find:
	find . -type f -print -exec grep $(word) {} \; | less # or -name '*.cc'
