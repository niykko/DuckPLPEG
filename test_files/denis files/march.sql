DROP FUNCTION IF EXISTS march(|vec2|);
CREATE FUNCTION march(start |vec2|) RETURNS SETOF |vec2| AS
$$
  DECLARE
    "track?" |boolean|  := |false|;
    goal     |vec2|;
    current_  |vec2|    := |start|;
    square   |squares|;
    dir      |directions|;
  BEGIN
    WHILE |true| LOOP
      IF |"track?" AND current_ = goal| THEN
        EXIT;
      END IF;

      -- Options:
      -- ➊ merge squares + directions queries
      -- ➋ unfold computation of squares here
      square := |(SELECT s
                 FROM   squares AS s
                 WHERE  s.xy = current_)|;
      dir := |(SELECT d
              FROM   directions AS d
              WHERE  (square.ll, square.lr, square.ul, square.ur) = (d.ll, d.lr, d.ul, d.ur))|;

      IF |NOT "track?" AND dir."track?"| THEN
        "track?" := |true|;
        goal     := |current_|;
      END IF;
      IF |"track?"| THEN
        RETURN NEXT |current_|;
      END IF;

      current_ := |(current_.x + (dir.dir).x, current_.y + (dir.dir).y) :: vec2|;
    END LOOP;

  END;
$$
LANGUAGE PLPGSQL STRICT;
