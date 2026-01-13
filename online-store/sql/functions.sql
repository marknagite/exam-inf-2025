CREATE OR REPLACE FUNCTION getOrderStatus(p_order_id INTEGER) RETURNS TEXT AS $$
DECLARE res TEXT;
BEGIN
  SELECT status INTO res FROM orders WHERE order_id = p_order_id;
  RETURN res;
END;
$$ LANGUAGE plpgsql;

CREATE OR REPLACE FUNCTION getOrderStatusHistory(p_order_id INTEGER) RETURNS TABLE(history_id INTEGER, old_status TEXT, new_status TEXT, changed_at TIMESTAMP, changed_by INTEGER) AS $$
BEGIN
  RETURN QUERY SELECT history_id, old_status, new_status, changed_at, changed_by FROM order_status_history WHERE order_id = p_order_id ORDER BY changed_at;
END;
$$ LANGUAGE plpgsql;

CREATE OR REPLACE FUNCTION getUserOrderCount() RETURNS TABLE(user_id INTEGER, cnt INTEGER) AS $$
BEGIN
  RETURN QUERY SELECT user_id, COUNT(*) FROM orders GROUP BY user_id;
END;
$$ LANGUAGE plpgsql;

CREATE OR REPLACE FUNCTION getTotalSpentByUser(p_user_id INTEGER) RETURNS NUMERIC AS $$
DECLARE s NUMERIC;
BEGIN
  SELECT COALESCE(SUM(total_price),0) INTO s FROM orders WHERE user_id = p_user_id;
  RETURN s;
END;
$$ LANGUAGE plpgsql;

CREATE OR REPLACE FUNCTION canReturnOrder(p_order_id INTEGER) RETURNS BOOLEAN AS $$
DECLARE st TEXT;
DECLARE od TIMESTAMP;
BEGIN
  SELECT status, order_date INTO st, od FROM orders WHERE order_id = p_order_id;
  IF st <> 'completed' THEN
    RETURN FALSE;
  END IF;
  IF od < NOW() - INTERVAL '30 days' THEN
    RETURN FALSE;
  END IF;
  RETURN TRUE;
END;
$$ LANGUAGE plpgsql;

CREATE OR REPLACE FUNCTION getAuditLogByUser(p_user_id INTEGER) RETURNS TABLE(log_id INTEGER, entity_type TEXT, entity_id INTEGER, operation TEXT, performed_at TIMESTAMP) AS $$
BEGIN
  RETURN QUERY SELECT log_id, entity_type, entity_id, operation, performed_at FROM audit_log WHERE performed_by = p_user_id ORDER BY performed_at;
END;
$$ LANGUAGE plpgsql;
