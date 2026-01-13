CREATE OR REPLACE FUNCTION trg_order_status_change() RETURNS TRIGGER AS $$
BEGIN
  IF TG_OP = 'UPDATE' THEN
    IF NEW.status IS DISTINCT FROM OLD.status THEN
      INSERT INTO order_status_history(order_id,old_status,new_status,changed_by) VALUES (NEW.order_id,OLD.status,NEW.status,NULL);
    END IF;
  END IF;
  RETURN NEW;
END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER order_status_change AFTER UPDATE ON orders FOR EACH ROW EXECUTE PROCEDURE trg_order_status_change();

CREATE OR REPLACE FUNCTION trg_product_price_change() RETURNS TRIGGER AS $$
BEGIN
  IF TG_OP = 'UPDATE' THEN
    IF NEW.price IS DISTINCT FROM OLD.price THEN
      UPDATE orders SET total_price = (
        SELECT COALESCE(SUM(oi.quantity * oi.price),0) FROM order_items oi WHERE oi.order_id = orders.order_id
      );
    END IF;
  END IF;
  RETURN NEW;
END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER product_price_change AFTER UPDATE ON products FOR EACH ROW EXECUTE PROCEDURE trg_product_price_change();
CREATE OR REPLACE FUNCTION trg_audit_generic() RETURNS TRIGGER AS $$
BEGIN
  IF TG_OP = 'INSERT' THEN
    INSERT INTO audit_log(entity_type,entity_id,operation,performed_by) VALUES (TG_TABLE_NAME, NEW.*::record::text::integer, 'insert', NULL);
  ELSIF TG_OP = 'UPDATE' THEN
    INSERT INTO audit_log(entity_type,entity_id,operation,performed_by) VALUES (TG_TABLE_NAME, NEW.*::record::text::integer, 'update', NULL);
  ELSIF TG_OP = 'DELETE' THEN
    INSERT INTO audit_log(entity_type,entity_id,operation,performed_by) VALUES (TG_TABLE_NAME, OLD.*::record::text::integer, 'delete', NULL);
  END IF;
  RETURN NEW;
END;
$$ LANGUAGE plpgsql;
