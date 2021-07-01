PREFIX foaf:    <http://xmlns.com/foaf/0.1/>
SELECT ?x ?z ?y ?nickY ?g
    WHERE
  { ?x foaf:knows ?z .
    ?z foaf:name ?nameX .
    ?y foaf:nick ?nickY .
    ?g foaf:single1 ?h .
  }



--x和z 有path y和nickY有path g和h  所有笛卡尔积
-- ------------------------------------
-- | x   | z   | y   | nickY    | g   |
-- ====================================
-- | <b> | <c> | <d> | "CTDDDD" | <e> |
-- | <b> | <c> | <d> | "CTDDDD" | <f> |
-- | <b> | <c> | <c> | "CT"     | <e> |
-- | <b> | <c> | <c> | "CT"     | <f> |
-- | <a> | <d> | <d> | "CTDDDD" | <e> |
-- | <a> | <d> | <d> | "CTDDDD" | <f> |
-- | <a> | <d> | <c> | "CT"     | <e> |
-- | <a> | <d> | <c> | "CT"     | <f> |
-- | <a> | <b> | <d> | "CTDDDD" | <e> |
-- | <a> | <b> | <d> | "CTDDDD" | <f> |
-- | <a> | <b> | <c> | "CT"     | <e> |
-- | <a> | <b> | <c> | "CT"     | <f> |
-- ------------------------------------

