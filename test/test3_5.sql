PREFIX foaf:    <http://xmlns.com/foaf/0.1/>
SELECT ?x ?z ?y ?nickY ?g
    WHERE
  { ?x foaf:knows ?z .
    ?z foaf:name ?nameX .
    ?y foaf:nick ?nickY .
    ?g foaf:cro "s5" .
  }

--   --x和z 有path y和nickY有path 单g  所有笛卡尔积
-- -------------------
-- 0|   ?x             ?z             ?y             ?nickY         ?g
-- 1|   <b>            <c>            <d>            "CTDDDD"       <j>
-- 2|   <b>            <c>            <c>            "CT"           <j>
-- 3|   <a>            <b>            <d>            "CTDDDD"       <j>
-- 4|   <a>            <b>            <c>            "CT"           <j>
-- 5|   <a>            <d>            <d>            "CTDDDD"       <j>
-- 6|   <a>            <d>            <c>            "CT"           <j>
-- 7|   <b>            <c>            <d>            "CTDDDD"       <i>
-- 8|   <b>            <c>            <c>            "CT"           <i>
-- 9|   <a>            <b>            <d>            "CTDDDD"       <i>
-- 10|  <a>            <b>            <c>            "CT"           <i>
-- 11|  <a>            <d>            <d>            "CTDDDD"       <i>
-- 12|  <a>            <d>            <c>            "CT"           <i>



