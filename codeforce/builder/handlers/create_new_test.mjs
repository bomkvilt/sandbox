import handlerRoot from './common.mjs'
import mustache from 'mustache'
import path from 'path';
import fs from 'fs';

mustache.escape = (value) => value;


export default 
class CreateNewTest 
{
    constructor(root, complexity, testID, testName, bTests) {
        this.clearName = this.CreateClearName(testName);
        this.fullName = this.CreateFullname(complexity, testID, testName);
        this.fileName = this.fullName + '.hpp';
        this.testName = this.fullName + '.test.cpp';
        this.filePath = path.join(root, complexity.toString(), this.fileName);
        this.testPath = path.join(root, complexity.toString(), this.testName);

        this.CreateFile();
        if (bTests) {
            this.CreateTest();
        }
    }

    CreateFullname(complexity, testID, testName) {
        return '' + complexity + '-' + testID + ' - ' + testName + ''
    }

    CreateClearName(testName) {
        let words = testName.split(/[\s-]/g).filter(el => el !== '');
        let parts = [];
        for (let word of words) {
            let part = word.replace(/\W/g, '');
            let capd = part.charAt(0).toUpperCase() + part.slice(1);
            parts.push(capd);
        }
        return parts.join('')
    }

    CreateFile() {
        let tmpl = this.ReadTemplate('file.tmpl.hpp');
        let text = mustache.render(tmpl, {
            problem: this.clearName,
        });
        this.WriteFile(this.filePath, text);
    }

    CreateTest() {
        let tmpl = this.ReadTemplate('test.tmpl.cpp');
        let text = mustache.render(tmpl, {
            file: this.fileName,
        });
        this.WriteFile(this.testPath, text);
    }

    ReadTemplate(name) {
        return fs.readFileSync(path.join(handlerRoot, '../templates', name), 'utf8');
    }

    WriteFile(dest, text) {
        let dir = path.dirname(dest);
        if (!fs.existsSync(dest)) {
            fs.mkdirSync(dir, {recursive: true});
        }
        fs.writeFileSync(dest, text);
    }
} 
