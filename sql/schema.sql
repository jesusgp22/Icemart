CREATE TABLE IF NOT EXISTS Food(
	id INTEGER PRIMARY KEY AUTOINCREMENT, 
        name VARCHAR(32) UNIQUE,
        measure_unit VARCHAR(16),
        weekly_consumption FLOAT DEFAULT 0
);

CREATE TABLE IF NOT EXISTS Recipe (
	id INTEGER PRIMARY KEY AUTOINCREMENT, 
	name VARCHAR(64),
	recipe_type VARCHAR(32),
	preparation TEXT,
        score TINYINT DEFAULT 5,
	vegetarian BOOLEAN,
        #dificulty VARCHAR(16),
        #prep_time VARCHAR(10),
        cals FLOAT DEFAULT 0,
        prots FLOAT DEFAULT 0,
        carbs FLOAT DEFAULT 0,
        fats FLOAT DEFAULT 0,
        last_cooked DATE DEFAULT NULL
);

CREATE TABLE IF NOT EXISTS Ingredient (
        id INTEGER PRIMARY KEY,
        food_id,
        recipe_id,
        amount INT Default 0,
        verbose_name VARCHAR(64),
        #PRIMARY KEY (food_id,recipe_id),
        FOREIGN KEY (food_id) REFERENCES Food(id),
        FOREIGN KEY (recipe_id) REFERENCES Recipe(id)
);

CREATE TABLE IF NOT EXISTS VerboseIngredient (
    id INTEGER PRIMARY KEY,
    recipe_id,
    verbose_name VARCHAR(64),
    FOREIGN KEY (recipe_id) REFERENCES Recipe(id)
);


CREATE TABLE IF NOT EXISTS Item (
	id INTEGER PRIMARY KEY AUTOINCREMENT, 
	expiration_date Date,
        amount INT,
	food_id INTEGER,
        FOREIGN KEY (food_id) REFERENCES Food(id)
);

CREATE TABLE IF NOT EXISTS MarketList (
        id INTEGER PRIMARY KEY AUTOINCREMENT,
        amount INTEGER,
        food_id INTEGER UNIQUE,
        FOREIGN KEY (food_id) REFERENCES Food(id)
);

CREATE TABLE IF NOT EXISTS Consumption(
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    amount INT,
    food_id INT,
    consumption_date DATE,
    FOREIGN KEY (food_id) REFERENCES Food(id)
);
