for %%k in (1000,2000,3000,4000,5000,6000,7000,8000,9000,10000,11000,12000,13000,14000,15000,16000,17000,18000,19000,20000) do decr.exe  Email-Enron.txt.snap  %%k  >> decr-Email-Enron.csv
for %%k in (1000,2000,3000,4000,5000,6000,7000,8000,9000,10000,11000,12000,13000,14000,15000,16000,17000,18000,19000,20000) do decr.exe com-dblp.ungraph.txt.snap  %%k  >> decr-com-dblp.ungraph.csv
for %%k in (1000,2000,3000,4000,5000,6000,7000,8000,9000,10000,11000,12000,13000,14000,15000,16000,17000,18000,19000,20000) do decr.exe com-youtube.ungraph.txt.snap  %%k  >> decr-com-youtube.ungraph.csv
for %%k in (1000,2000,3000,4000,5000,6000,7000,8000,9000,10000,11000,12000,13000,14000,15000,16000,17000,18000,19000,20000) do decr.exe com-lj.ungraph.txt.snap  %%k  >> decr-com-lj.ungraph.csv

for %%k in (1000,2000,3000,4000,5000,6000,7000,8000,9000,10000,11000,12000,13000,14000,15000,16000,17000,18000,19000,20000) do incr.exe  Email-Enron.txt.snap  %%k  >> incr-Email-Enron.csv
for %%k in (1000,2000,3000,4000,5000,6000,7000,8000,9000,10000,11000,12000,13000,14000,15000,16000,17000,18000,19000,20000) do incr.exe com-dblp.ungraph.txt.snap  %%k  >> incr-com-dblp.ungraph.csv
for %%k in (1000,2000,3000,4000,5000,6000,7000,8000,9000,10000,11000,12000,13000,14000,15000,16000,17000,18000,19000,20000) do incr.exe com-youtube.ungraph.txt.snap  %%k  >> incr-com-youtube.ungraph.csv
for %%k in (1000,2000,3000,4000,5000,6000,7000,8000,9000,10000,11000,12000,13000,14000,15000,16000,17000,18000,19000,20000) do incr.exe com-lj.ungraph.txt.snap  %%k  >> incr-com-lj.ungraph.csv

rem for %%k in (10,15,20,25,30,35,40,45,50,100,200,300,400,500,1000) do	for %%r in (10000,20000,30000,40000,50000,60000,70000,80000,90000,100000,200000) do bigsize.exe com-dblp.ungraph.txt.snap  %%r %%k >> bigsize-com-dblp.ungraph.csv
rem for %%k in (10,15,20,25,30,35,40,45,50,100,200,300,400,500,1000) do	for %%r in (10000,20000,30000,40000,50000,60000,70000,80000,90000,100000,200000) do bigsize.exe com-youtube.ungraph.txt.snap  %%r %%k >> bigsize-com-youtube.ungraph.csv
rem for %%k in (10,15,20,25,30,35,40,45,50,100,200,300,400,500,1000) do	for %%r in (10000,20000,30000,40000,50000,60000,70000,80000,90000,100000,200000) do bigsize.exe com-lj.ungraph.txt.snap  %%r %%k >> bigsize-com-lj.ungraph.csv
rem for %%k in (10,15,20,25,30,35,40,45,50,100,200,300,400,500,1000) do	for %%r in (10000,20000,30000,40000,50000,60000,70000,80000,90000,100000,200000) do bigsize.exe Email-Enron.txt.snap  %%r %%k >> bigsize-Email-Enron.csv