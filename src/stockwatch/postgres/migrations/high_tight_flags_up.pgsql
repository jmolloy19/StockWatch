CREATE TABLE IF NOT EXISTS high_tight_flags (
    symbol VARCHAR(5) PRIMARY KEY,
    flag_pole_bottom_timestamp BIGINT NOT NULL CHECK (flag_pole_bottom_timestamp >= 0),
    flag_pole_top_timestamp BIGINT NOT NULL CHECK (flag_pole_top_timestamp >= 0),
    breakout_price FLOAT(2) NOT NULL CHECK (breakout_price >= 0)
);

-- CREATE TABLE IF NOT EXISTS high_tight_flag_results (
--     symbol TEXT PRIMARY KEY,
--     mic_code TEXT NOT NULL,
--     flag_pole_bottom_timestamp BIGINT NOT NULL CHECK (flag_pole_bottom_timestamp >= 0),
--     flag_pole_top_timestamp BIGINT NOT NULL CHECK (flag_pole_top_timestamp >= 0),
--     breakout_price FLOAT(2) NOT NULL CHECK (breakout_price >= 0),
--     broke_out BOOLEAN NOT NULL,
--     six_week_close_max FLOAT(2) NOT NULL CHECK (six_week_close_max >= 0),
--     price_difference_percent FLOAT(4) NOT NULL,
--     flag_pole_distance INT NOT NULL CHECK (flag_pole_distance >= 0),
--     flag_pole_avg_volume INT CHECK (flag_pole_distance >= 0),
--     flag_avg_volume INT CHECK (flag_pole_distance >= 0),
--     volume_difference_percent FLOAT(4) NOT NULL,
-- );
