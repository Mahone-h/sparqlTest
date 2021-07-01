PREFIX foaf:    <http://xmlns.com/foaf/0.1/>
SELECT ?x ?z ?y ?nickY ?g ?h
    WHERE
  { ?x foaf:knows ?z .
    ?z foaf:name ?nameX .
    ?y foaf:nick ?nickY .
    ?g foaf:single1 ?a .
    ?h foaf:single2 ?b .
  }
/*x和z 有path y和nickY有path 加单g 单h  所有笛卡尔积*/
-- -------------------
-- 0|   ?x             ?z             ?y             ?nickY         ?g             ?h
-- 1|   <b>            <c>            <d>            "CTDDDD"       <f>            <h>
-- 2|   <b>            <c>            <d>            "CTDDDD"       <f>            <g>
-- 3|   <b>            <c>            <d>            "CTDDDD"       <e>            <h>
-- 4|   <b>            <c>            <d>            "CTDDDD"       <e>            <g>
-- 5|   <b>            <c>            <c>            "CT"           <f>            <h>
-- 6|   <b>            <c>            <c>            "CT"           <f>            <g>
-- 7|   <b>            <c>            <c>            "CT"           <e>            <h>
-- 8|   <b>            <c>            <c>            "CT"           <e>            <g>
-- 9|   <a>            <b>            <d>            "CTDDDD"       <f>            <h>
-- 10|  <a>            <b>            <d>            "CTDDDD"       <f>            <g>
-- 11|  <a>            <b>            <d>            "CTDDDD"       <e>            <h>
-- 12|  <a>            <b>            <d>            "CTDDDD"       <e>            <g>
-- 13|  <a>            <b>            <c>            "CT"           <f>            <h>
-- 14|  <a>            <b>            <c>            "CT"           <f>            <g>
-- 15|  <a>            <b>            <c>            "CT"           <e>            <h>
-- 16|  <a>            <b>            <c>            "CT"           <e>            <g>
-- 17|  <a>            <d>            <d>            "CTDDDD"       <f>            <h>
-- 18|  <a>            <d>            <d>            "CTDDDD"       <f>            <g>
-- 19|  <a>            <d>            <d>            "CTDDDD"       <e>            <h>
-- 20|  <a>            <d>            <d>            "CTDDDD"       <e>            <g>
-- 21|  <a>            <d>            <c>            "CT"           <f>            <h>
-- 22|  <a>            <d>            <c>            "CT"           <f>            <g>
-- 23|  <a>            <d>            <c>            "CT"           <e>            <h>
-- 24|  <a>            <d>            <c>            "CT"           <e>            <g>





