Two physical facts drive everything:

Signal strength (link margin / RSSI) can only be measured by the receiver of a frame.
Error rate can only be measured by the transmitter (it counts missing acks). A receiver can't count frames it never received.
In/Out and Rx/Tx always name the link direction as seen by the reporting Router — never who measured it. So for each direction, the two metrics come from opposite ends of the link:

| TLV | Direction | Measured by | Valid in |
|-----|-----------|-------------|----------|
| Link Margin In | peer → Router | Router | Child, Neighbor |
| MAC Link Error Rates Tx | Router → peer | Router | Child, Neighbor |
| MAC Link Error Rates Rx | child → Router | child | Child only |
| Link Margin Out | Router → child | child | Child only |


The two child-measured ones are exactly the two directions the Router physically cannot observe, which is why the child measures them, sends them to its parent, and the parent forwards them in the child context. That's also why they're child-context-only.