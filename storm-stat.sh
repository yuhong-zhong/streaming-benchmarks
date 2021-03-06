FILE=/tmp/storm
printf "Report average latency to %s\n" $FILE

ID=$(curl -s http://127.0.0.1:8080/api/v1/topology/summary | jq -r '.["topologies"][0]["id"]')
printf "Topology ID: %s\n" $ID
while :
do
    LATENCY=$(curl -s http://127.0.0.1:8080/api/v1/topology/$ID | jq -r '.["topologyStats"][0]["completeLatency"]')
    printf "Average latency: %s ms\n" $LATENCY
    ./atomic_write $LATENCY $FILE
    sleep 1
done