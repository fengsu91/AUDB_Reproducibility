CREATE OR REPLACE FUNCTION stringdist (TEXT, TEXT) RETURNS FLOAT
AS
$$
  DECLARE
    l ALIAS FOR $1;
    r ALIAS FOR $2;
    llen INT;
    rlen INT;
    maxlen INT;
    minlen INT;
    lchar CHAR;
    rchar CHAR;
    weight FLOAT;
    pos INT;
    res FLOAT;
    diff FLOAT;
  BEGIN
    pos := 1;
    res := 0.0;
    llen := length(l);
    rlen := length(r);
    maxlen := CASE WHEN llen < rlen THEN rlen ELSE llen END;
    minlen := CASE WHEN llen < rlen THEN llen ELSE rlen END;
    WHILE pos <= minlen LOOP
      lchar := substring(l,pos,pos)::char;
      rchar := substring(r,pos,pos)::char;
      weight := power(10, -1 * (pos - 1));
      diff := ASCII(rchar) - ASCII(lchar);
      res := res + ((weight*diff) *(1.0 / 256));
      pos := pos + 1;
    END LOOP;
    IF rlen = maxlen THEN
      WHILE pos <= maxlen LOOP
        rchar := substring(r,pos,pos)::char;
        weight := power(10, -1 * (pos - 1));
        diff := ASCII(rchar);
        res := res + ((weight::float*diff)*(1.0 / 256));
        pos := pos + 1;
      END LOOP;
    ELSE
      WHILE pos <= maxlen LOOP
        lchar := substring(l,pos,pos)::char;
        weight := power(10, -1 * (pos - 1));
        diff := 256 - ASCII(lchar);
        res := res + ((weight::float*diff)*(1.0 / 256));
        pos := pos + 1;
      END LOOP;
    END IF;
    RETURN res;
  END;
$$ LANGUAGE PLPGSQL;