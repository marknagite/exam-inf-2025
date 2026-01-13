CREATE TABLE users (
  user_id SERIAL PRIMARY KEY,
  name VARCHAR(255) NOT NULL,
  email VARCHAR(255) NOT NULL UNIQUE,
  role VARCHAR(50) NOT NULL,
  password_hash VARCHAR(255),
  loyalty_level INTEGER DEFAULT 0
);

CREATE TABLE products (
  product_id SERIAL PRIMARY KEY,
  name VARCHAR(255) NOT NULL,
  price NUMERIC CHECK (price > 0),
  stock_quantity INTEGER CHECK (stock_quantity >= 0)
);

CREATE TABLE orders (
  order_id SERIAL PRIMARY KEY,
  user_id INTEGER REFERENCES users(user_id),
  status VARCHAR(50) NOT NULL,
  total_price NUMERIC,
  order_date TIMESTAMP DEFAULT NOW()
);

CREATE TABLE order_items (
  order_item_id SERIAL PRIMARY KEY,
  order_id INTEGER REFERENCES orders(order_id) ON DELETE CASCADE,
  product_id INTEGER REFERENCES products(product_id),
  quantity INTEGER NOT NULL,
  price NUMERIC NOT NULL
);

CREATE TABLE order_status_history (
history_id SERIAL PRIMARY KEY,
  order_id INTEGER REFERENCES orders(order_id),
  old_status VARCHAR(50),
  new_status VARCHAR(50),
  changed_at TIMESTAMP DEFAULT NOW(),
  changed_by INTEGER REFERENCES users(user_id)
);

CREATE TABLE audit_log (
  log_id SERIAL PRIMARY KEY,
  entity_type VARCHAR(50),
  entity_id INTEGER,
  operation VARCHAR(50),
  performed_by INTEGER,
  performed_at TIMESTAMP DEFAULT NOW()
);
