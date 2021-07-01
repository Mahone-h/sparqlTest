PREFIX foaf:    <http://xmlns.com/foaf/0.1/>
SELECT ?x ?y
    WHERE
  {
    ?x foaf:cro "s5" .
    ?y foaf:cro "s6".
  }



/*测双iri*/
----------------------------
/*0|   ?x             ?y
1|   <j>            <l>
2|   <j>            <k>
3|   <i>            <l>
4|   <i>            <k>    */