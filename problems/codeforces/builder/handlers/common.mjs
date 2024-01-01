const currentFilename = 'common.mjs';

const handlerRoot = (() => {
    try { throw new Error();
    } catch (e) {
        const msg = e.stack.toString();
        const bgn = msg.indexOf('file:///') + 8;
        const end = msg.indexOf('/' + currentFilename);
        const dir = msg.substr(bgn, end - bgn).replace(/\//g, '\\');
        return dir;
    }
})();

export {handlerRoot as default};
