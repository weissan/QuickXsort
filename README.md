# QuickXsort

This repository contains the QuickMergesort variants examined in the papers "QuickXsort -- A Fast Sorting Scheme in Theory and Practice" by Stefan Edelkamp, Armin Weiß and Sebastian Wild and "Worst-Case Efficient Sorting with QuickMergesort" by Stefan Edelkamp and Armin Weiß. 


- For running time experiments with one single algorithm (the algorithm names are the file names of the \*.h++ files:
	make <algorithm>.time
  
- For running time and comparison measurements of "Worst-Case Efficient Sorting with QuickMergesort" (output  written to .csv file):
   - make undersamplingcomptest
	- make mqmsOnlycomptest
	- make mqmsOnlytimetest
	- make mqmsAlgtimetesttypes
	- make mqmsHybridtimetest
  
- For running time and comparison measurements of "QuickXsort -- A Fast Sorting Scheme in Theory and Practice" (output  written to .csv file):
	 - make qXcomptest
	- make qXcomptestsmalldense
	- make qXtimetest
	- make singlerepetitiontimetest  (for variance measurements)
	- make singlerepetitioncomptest  (for variance measurements)

- For comparison and move experiments with one single algorithm:
	make <algorithm>.comp
	make <algorithm>.move
  These tests are not implemented for all algorithms.

