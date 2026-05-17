CREATE TABLE users (
    id serial PRIMARY KEY,
    name text
);

INSERT INTO users(name)
VALUES
('John'),
('Jane'),
('Bob');
