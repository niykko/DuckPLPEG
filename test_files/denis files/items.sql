DROP FUNCTION IF EXISTS count_items(INT8);
CREATE OR REPLACE FUNCTION count_items(catid INT8) RETURNS INT8 AS
$$
DECLARE
  totalcount INT8;
  curcat INT8;
  catitems INT8;
  subcat INT8;
  stack INT8[];
  catrec category;
BEGIN
  totalcount := $0 :: INT8$;
  stack := $ARRAY[catid]$;

  WHILE $cardinality(stack) > 0$ LOOP
    curcat := stack[1];
    stack := stack[2:];
    catitems := $(SELECT count(P_PARTKEY)
                 FROM   item
                 WHERE category_id = curcat)$;
    totalcount := $totalcount + catitems$;
    stack := $(SELECT array_agg(category_id :: INT8)
              FROM category
              WHERE parent_category = curcat) || stack$;
  END LOOP;
  RETURN totalcount;
END
$$ LANGUAGE PLPGSQL;