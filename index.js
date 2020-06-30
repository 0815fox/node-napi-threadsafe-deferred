const join = require('path').join;

module.exports = {
    include: join(__dirname, "./src"),
    gyp: join(__dirname, './binding.gyp:napi-threadsafe-deferred')
};
