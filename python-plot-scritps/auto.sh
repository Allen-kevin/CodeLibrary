python wrk-test-parse.py -i ./data/kernel-http.txt -o rps.txt
python wrk-test-parse.py -i ./data/kernel-https.txt -o rps.txt
python wrk-test-parse.py -i ./data/wrk-http.txt -o rps.txt
python wrk-test-parse.py -i ./data/wrk-t10-high5-low95-rps.txt -o rps.txt
#python wrk-test-parse.py -i wrk_nginx_https.txt -o https.txt
#python monitor-test-parse.py -i temp.txt -o latency.txt

#python rps_plot.py
