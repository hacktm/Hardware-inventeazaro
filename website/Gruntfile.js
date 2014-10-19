/*
 * Generated on 2014-10-18
 * generator-assemble v0.5.0
 * https://github.com/assemble/generator-assemble
 *
 * Copyright (c) 2014 Hariadi Hinta
 * Licensed under the MIT license.
 */

'use strict';

// # Globbing
// for performance reasons we're only matching one level down:
// '<%= config.src %>/templates/pages/{,*/}*.hbs'
// use this if you want to match all subfolders:
// '<%= config.src %>/templates/pages/**/*.hbs'

module.exports = function(grunt) {

  require('time-grunt')(grunt);
  require('load-grunt-tasks')(grunt);

  // Project configuration.
  grunt.initConfig({

    shell: {
      options: {
         stderr: false
      },
      target: {
         command: '../update-site.sh'
      }
    },
    config: {
      src: 'src',
      dist: 'dist'
    },

    watch: {
      assemble: {
      files: ['<%= config.src %>/{content,data,templates}/{,*/}*.{md,hbs,yml}'],
        tasks: ['assemble']
      },
      assets: {
        files: ['<%= config.src %>/css/*.css'],
        tasks: ['copy:theme']
      },
      js: {
        files: ['<%= config.src %>/js/*.js'],
        tasks: ['copy:js']
      },
      livereload: {
        options: {
          livereload: '<%= connect.options.livereload %>'
        },
        files: [
          '<%= config.dist %>/{,*/}*.html',
          '<%= config.dist %>/assets/{,*/}*.css',
          '<%= config.dist %>/assets/{,*/}*.js',
          '<%= config.dist %>/assets/{,*/}*.{png,jpg,jpeg,gif,webp,svg}'
        ]
      }
    },

    connect: {
      options: {
        port: 9000,
        livereload: 35729,
        // change this to '0.0.0.0' to access the server from outside
        hostname: 'localhost'
      },
      livereload: {
        options: {
          open: true,
          base: [
            '<%= config.dist %>'
          ]
        }
      }
    },

    assemble: {
      pages: {
        options: {
          flatten: true,
          assets: '<%= config.dist %>/assets',
          layout: '<%= config.src %>/templates/layouts/default.hbs',
          data: '<%= config.src %>/data/*.{json,yml}',
          partials: '<%= config.src %>/templates/partials/*.hbs'
        },
        files: {
          '<%= config.dist %>/': ['<%= config.src %>/templates/pages/*.hbs']
        }
      }
    },

    copy: {
      bootstrap: {
        expand: true,
        cwd: 'bower_components/bootstrap/dist/',
        src: '**',
        dest: '<%= config.dist %>/assets/'
      },
      xeditable: {
        expand: true,
        cwd: 'bower_components/x-editable/dist/bootstrap3-editable/',
        src: '**',
        dest: '<%= config.dist %>/assets/'
      },
      d3: {
        expand: true,
        cwd: 'bower_components/d3/',
        src: 'd3.js',
        dest: '<%= config.dist %>/assets/js/'
      },
      theme: {
        expand: true,
        cwd: 'src/css/',
        src: '**',
        dest: '<%= config.dist %>/assets/css/'
      },
      img: {
        expand: true,
        cwd: 'src/img/',
        src: '**',
        dest: '<%= config.dist %>/assets/img/'
      },
      js: {
       expand: true,
       cwd: 'src/js/',
       src: '**',
       dest: '<%= config.dist %>/js/'
     },
     font: {
       expand: true,
       cwd: 'src/../fonts/',
       src: '**',
       dest: '<%= config.dist %>/../fonts/'
     }
    },

    // Before generating any new files,
    // remove any previously-created files.
    clean: ['<%= config.dist %>/**/*.{html,xml}']

  });

  grunt.loadNpmTasks('assemble');

  grunt.registerTask('server', [
    'build',
    'connect:livereload',
    'watch'
  ]);
  grunt.registerTask('release', [
    'build',
    'shell'

  ]);


  grunt.registerTask('build', [
    'clean',
    'copy',
    'assemble'
  ]);

  grunt.registerTask('default', [
    'build'
  ]);

};
