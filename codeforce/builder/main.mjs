import CreateNewTest from './handlers/create_new_test.mjs'
import ForceToDebug  from './handlers/force_to_debug.mjs'
import yargs from 'yargs';

yargs.command({
    command:    'add <complexity> <testID> <testName>',
    desc:       'creates new problem files',
    handler:    (argv ) => new CreateNewTest(argv.root, argv.complexity, argv.testID, argv.testName, argv.tests),
    builder:    (yargs) => yargs
        .positional('complexity', {
            desc: 'codeforces` test complexity',
            type: 'number'
        })
        .positional('testID', {
            desc: 'codeforces` test id',
            type: 'number'
        })
        .positional('testName', {
            desc: 'human-readable test name',
            type: 'string'
        }).option('tests', {
            alias: 't',
            type: 'boolean',
            default: true,
            desc: 'create a unit test file'
        }).option('root', {
            alias: 'r',
            type: 'string',
            desc: 'path to code directory`s root',
            required: true,
        })
}).command({
    command:    'test <testID>',
    desc:       'set a problem with the @testID to cpp-main project',
    handler:    (argv) => new ForceToDebug(argv.testID, argv.root, argv.targetFile),
    builder:    (yargs) => yargs
        .positional('testID', {
            desc: 'selected problem codeforces` ID',
            type: 'number'
        }).option('root', {
            alias: 'r',
            type: 'string',
            desc: 'path to code directory`s root',
            required: true,
        }).option('targetFile', {
            alias: 'tf',
            type: 'string',
            desc: 'path to debug project`s target file',
            required: true
        })
}).parse()
