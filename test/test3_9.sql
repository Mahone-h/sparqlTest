PREFIX foaf:    <http://xmlns.com/foaf/0.1/>
SELECT ?x ?y ?nameX ?nickY
    WHERE
 { { ?x foaf:knows ?y .
   ?y foaf:name ?nameX .}
UNION
{ ?y foaf:nick ?nickY }}


-- -----------------------------------
-- | x   | y   | nameX    | nickY    |
-- ===================================
-- | <b> | <c> | "Clare"  |          |
-- | <a> | <d> | "dddddd" |          |
-- | <a> | <b> | "Bob"    |          |
-- |     | <d> |          | "CTDDDD" |
-- |     | <c> |          | "CT"     |
-- -----------------------------------