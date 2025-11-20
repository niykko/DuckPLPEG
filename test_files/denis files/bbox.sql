CREATE FUNCTION bbox(start vec2) RETURNS box AS
$$
  DECLARE
    "track?" boolean  := false;
    goal     vec2;
    bbox     box      := NULL;
    current_  vec2     := start;
    square   squares;
    dir      directions;
  BEGIN
    WHILE true LOOP
      IF "track?" AND $current_ = goal$ THEN
        EXIT;
      END IF;

      -- Options:
      -- ➊ merge squares + directions queries
      -- ➋ unfold computation of squares here
      square := $(SELECT s
                 FROM   squares AS s
                 WHERE  s.xy = current_)$;
      dir := $(SELECT d
              FROM   directions AS d
              WHERE  (square.ll, square.lr, square.ul, square.ur) = (d.ll, d.lr, d.ul, d.ur))$;

      IF NOT "track?" AND $dir."track?"$ THEN
        "track?" := true;
        goal     := current_;
        bbox     := $box(point(goal.x, goal.y))$;
      END IF;
      IF "track?" THEN
        bbox := $bound_box(bbox, box(point(current_.x, current_.y)))$;
      END IF;

      current_ := $current_.x + (dir.dir).x, current_.y + (dir.dir).y) :: vec2$;
    END LOOP;

    RETURN bbox;
  END;
$$
LANGUAGE PLPGSQL STRICT;