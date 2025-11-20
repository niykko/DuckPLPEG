CREATE FUNCTION long_distance(orderkey int) RETURNS boolean AS
$$
  DECLARE
    regions $int[]$;
    region  int;
  BEGIN
    regions := $(SELECT array_agg(n.n_regionkey)
                FROM   lineitem AS l, supplier AS s, nation AS n
                WHERE  l.l_orderkey = orderkey
                AND    l.l_suppkey  = s.s_suppkey AND s.s_nationkey = n.n_nationkey)$;

    FOREACH region IN ARRAY regions LOOP
      IF $region <> regions[1]$ THEN RETURN true;
      END IF;
    END LOOP;

    RETURN false;
  END;
$$
LANGUAGE PLPGSQL;