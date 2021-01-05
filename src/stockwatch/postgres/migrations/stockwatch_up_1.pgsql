CREATE TABLE IF NOT EXISTS watched_patterns (
    symbol VARCHAR(5) NOT NULL,
    pattern VARCHAR(5) NOT NULL,
    pattern_start_timestamp BIGINT NOT NULL CHECK (pattern_start_timestamp >= 0),
    pattern_formation_timestamp BIGINT NOT NULL CHECK (pattern_formation_timestamp > pattern_start_timestamp),
    finished_watching_timestamp BIGINT NOT NULL CHECK (finished_watching_timestamp > pattern_formation_timestamp),
    breakout_price FLOAT(2) NOT NULL CHECK (breakout_price >= 0),
    PRIMARY KEY (symbol, pattern)
);

CREATE TABLE IF NOT EXISTS finished_patterns (
    symbol VARCHAR(5) NOT NULL,
    pattern VARCHAR(5) NOT NULL,
    mic_code VARCHAR(5) NOT NULL,
    pattern_start_timestamp BIGINT NOT NULL CHECK (pattern_start_timestamp >= 0),
    pattern_formation_timestamp BIGINT NOT NULL CHECK (pattern_formation_timestamp > pattern_start_timestamp),
    finished_watching_timestamp BIGINT NOT NULL CHECK (finished_watching_timestamp > pattern_formation_timestamp),
    breakout_price FLOAT(2) NOT NULL CHECK (breakout_price >= 0),
    max_price_from_formation_to_finish FLOAT(2) NOT NULL CHECK (breakout_price >= 0),
    max_percent_gain FLOAT(2) NOT NULL,
    PRIMARY KEY (symbol, pattern, pattern_start_timestamp)
);

CREATE TABLE IF NOT EXISTS candles (
    symbol VARCHAR(5) NOT NULL,
    close FLOAT(2) NOT NULL CHECK (close >= 0),
    open FLOAT(2) NOT NULL CHECK (open >= 0),
    high FLOAT(2) NOT NULL CHECK (high >= 0),
    low FLOAT(2) NOT NULL CHECK (low >= 0),
    volume INT NOT NULL CHECK (volume >= 0),
    timestamp BIGINT NOT NULL CHECK (timestamp >= 0),
    PRIMARY KEY (symbol, timestamp)
);
