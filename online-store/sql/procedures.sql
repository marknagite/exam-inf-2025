CREATE OR REPLACE PROCEDURE createOrder(p_user_id INTEGER, OUT p_order_id INTEGER) LANGUAGE plpgsql AS $$
DECLARE tot NUMERIC := 0;
BEGIN
  BEGIN
    INSERT INTO orders(user_id,status,total_price) VALUES (p_user_id,'pending',0) RETURNING order_id INTO p_order_id;
  EXCEPTION WHEN OTHERS THEN
    RAISE;
  END;
END;
$$;

CREATE OR REPLACE PROCEDURE changeOrderStatus(p_order_id INTEGER,p_new_status TEXT,p_changed_by INTEGER) LANGUAGE plpgsql AS $$
DECLARE old TEXT;
BEGIN
  SELECT status INTO old FROM orders WHERE order_id = p_order_id;
  UPDATE orders SET status = p_new_status, order_date = NOW() WHERE order_id = p_order_id;
  INSERT INTO order_status_history(order_id,old_status,new_status,changed_by) VALUES (p_order_id,old,p_new_status,p_changed_by);
  INSERT INTO audit_log(entity_type,entity_id,operation,performed_by) VALUES ('order',p_order_id,'update',p_changed_by);
END;
$$;
