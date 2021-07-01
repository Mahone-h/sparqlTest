PREFIX foaf:    <http://xmlns.com/foaf/0.1/>
SELECT ?x  ?y ?z ?nickY
    WHERE
  { ?x foaf:knows ?z .
    ?z foaf:name ?nameX .
    ?y foaf:nick ?nickY .
  }

/* x和z 有path y和nickY有path  他们之间是笛卡尔积 有无?nicky不影响  */
-- -------------------
-- 0|   ?x             ?y             ?z
-- 1|   <b>            <d>            <c>
-- 2|   <b>            <c>            <c>
-- 3|   <a>            <d>            <b>
-- 4|   <a>            <c>            <b>
-- 5|   <a>            <d>            <d>
-- 6|   <a>            <c>            <d>
-------------------
-- 0|   ?x             ?y             ?z             ?nickY
-- 1|   <b>            <d>            <c>            "CTDDDD"
-- 2|   <b>            <c>            <c>            "CT"
-- 3|   <a>            <d>            <b>            "CTDDDD"
-- 4|   <a>            <c>            <b>            "CT"
-- 5|   <a>            <d>            <d>            "CTDDDD"
-- 6|   <a>            <c>            <d>            "CT"