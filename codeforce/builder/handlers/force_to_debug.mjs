import globy from 'globby';
import path from 'path';
import fs from 'fs';
import { assert } from 'console';


export default
class ForceToDebug
{
    constructor(testID, root, targetFile) {
        let problemPath = this.FindProblem(testID, root);
        fs.writeFileSync(targetFile, problemPath);
    }

    FindProblem(testID, root) {
        let absRoot = path.resolve(root).replace(/\\/g, '/');
        let paths = globy.sync(`${absRoot}`);

        let pattern = new RegExp(`\\w+-` + testID + ` -.+\\.hpp`);
        let files = paths.filter(file => path.basename(file).match(pattern));
        assert(files.length == 1);
        return files[0];
    }
}
