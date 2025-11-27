DROP FUNCTION order_kept_waiting(|int|,|int|);
CREATE FUNCTION order_kept_waiting(suppkey |int|, orderkey |int|) RETURNS |boolean| AS
$$
  DECLARE
    lis   |lineitem[]|;
    li    |lineitem|;
    blame |boolean| := |false|; -- is suppkey to blame?
    multi |boolean| := |false|; -- does this order have multiple suppliers?
  BEGIN
    lis := |(SELECT array_agg(l)
            FROM   lineitem AS l
            WHERE  l.l_orderkey = orderkey)|;
    FOREACH li IN ARRAY |lis| LOOP
      multi := |multi OR li.l_suppkey <> suppkey|;
      IF |li.l_receiptdate > li.l_commitdate| THEN
          IF |li.l_suppkey <> suppkey| THEN
            <<label>> DECLARE -- i added this part to test inner labeled blocks
            some_other_supplier |boolean| := |false|; -- 
            BEGIN -- 
                NULL; --
            END label; --
            RETURN |false|;
          ELSE
            |blame := true|;
          END IF;
      END IF;
    END LOOP;
    RETURN |multi AND blame|;
  END;
$$
LANGUAGE PLPGSQL;