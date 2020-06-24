import path from 'path';

export const include = `"${__dirname}"./src`;
export const gyp = path.join(__dirname, './binding.gyp:napi-threadsafe-deferred');
