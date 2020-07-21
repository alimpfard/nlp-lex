const { env } = require('process');

module.exports = {
    port: env.PORT || 8080,
    mongo_addr: env.MONGO_ADDRESS || "mongodb://127.0.0.1",
    database: "nlex",
    nlexBinary: '../nlex/bin/nlex',
    // nlexBinary: '../../src/build/nlex',
    example_path: '../nlex/examples/',
    // example_path: '../../examples/',
};
