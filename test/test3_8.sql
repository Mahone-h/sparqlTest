PREFIX foaf:    <http://xmlns.com/foaf/0.1/>
SELECT ?x  ?y ?z ?nickY
    WHERE
  { ?x foaf:knows ?g .
    ?y foaf:name ?nameX .
    ?z foaf:nick ?nickY .
  }


/*x y 单  z和nickY有path */
-------------------
/*0|   ?x             ?y             ?z             ?nickY
1|   <b>            <d>            <d>            "CTDDDD"
2|   <b>            <d>            <c>            "CT"
3|   <b>            <c>            <d>            "CTDDDD"
4|   <b>            <c>            <c>            "CT"
5|   <b>            <b>            <d>            "CTDDDD"
6|   <b>            <b>            <c>            "CT"
7|   <b>            <a>            <d>            "CTDDDD"
8|   <b>            <a>            <c>            "CT"
9|   <a>            <d>            <d>            "CTDDDD"
10|  <a>            <d>            <c>            "CT"
11|  <a>            <c>            <d>            "CTDDDD"
12|  <a>            <c>            <c>            "CT"
13|  <a>            <b>            <d>            "CTDDDD"
14|  <a>            <b>            <c>            "CT"
15|  <a>            <a>            <d>            "CTDDDD"
16|  <a>            <a>            <c>            "CT"
17|  <a>            <d>            <d>            "CTDDDD"
18|  <a>            <d>            <c>            "CT"
19|  <a>            <c>            <d>            "CTDDDD"
20|  <a>            <c>            <c>            "CT"
21|  <a>            <b>            <d>            "CTDDDD"
22|  <a>            <b>            <c>            "CT"
23|  <a>            <a>            <d>            "CTDDDD"
24|  <a>            <a>            <c>            "CT"     */