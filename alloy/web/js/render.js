var camera, renderer, controls, container;

var mouseX = 0, mouseY = 0;

var windowHalfX = window.innerWidth / 2;
var windowHalfY = window.innerHeight / 2;

// Grid parameters
var GRID_COLOR = 0x888888;
var GRID_WIDTH = 1;
var GRID_CENTER_WIDTH = 2;
var GRID_Y = 0;
var GRID_SIZE = 500;
var HALF_GRID_SIZE = GRID_SIZE / 2;
var GRID_DIVISIONS = 50;
var GRID_UNIT_SIZE = GRID_SIZE / GRID_DIVISIONS;

init();
animate();

function addGrid(scene) {
    var line_material = new THREE.LineBasicMaterial( { color: GRID_COLOR,
                                                       linewidth: GRID_WIDTH } );
    var center_line_material = new THREE.LineBasicMaterial( { color: GRID_COLOR,
                                                       linewidth: GRID_CENTER_WIDTH } );
    var geometry = new THREE.Geometry();
    var center_geometry = new THREE.Geometry();

    // outer lines
    for(var i = 0; i <= GRID_DIVISIONS; i++) {
        geometry.vertices.push(new THREE.Vector3(- HALF_GRID_SIZE,
                                                 GRID_Y,
                                                 i * GRID_UNIT_SIZE - HALF_GRID_SIZE ));

        geometry.vertices.push(new THREE.Vector3(HALF_GRID_SIZE,
                                                 GRID_Y,
                                                 i * GRID_UNIT_SIZE - HALF_GRID_SIZE ));

        geometry.vertices.push(new THREE.Vector3(i * GRID_UNIT_SIZE - HALF_GRID_SIZE,
                                                 GRID_Y,
                                                 -HALF_GRID_SIZE ));
        geometry.vertices.push(new THREE.Vector3(i * GRID_UNIT_SIZE - HALF_GRID_SIZE,
                                                 GRID_Y,
                                                HALF_GRID_SIZE));
    }

    var line = new THREE.Line( geometry, line_material, THREE.LinePieces );
    scene.add( line );

    // Center lines that are thicker
    center_geometry.vertices.push(new THREE.Vector3(-HALF_GRID_SIZE,
                                                    GRID_Y,
                                                    0 ));

    center_geometry.vertices.push(new THREE.Vector3(HALF_GRID_SIZE,
                                                    GRID_Y,
                                                    0 ));

    center_geometry.vertices.push(new THREE.Vector3(0,
                                                    GRID_Y,
                                                    HALF_GRID_SIZE ));

    center_geometry.vertices.push(new THREE.Vector3(0,
                                                     GRID_Y,
                                                     -HALF_GRID_SIZE ));

    var center_line = new THREE.Line( center_geometry, center_line_material, THREE.LinePieces );
    scene.add( center_line );
}

function addVoxels(scene) {
    $.getJSON("lego.json" , function(result) {
        var colors = {};
        var voxelWidth = 50;
        var voxelHeight = (50 * 3.2) / 15.9;
        var voxelDepth = 50;
        var voxelGeometry = new THREE.BoxGeometry(voxelWidth, voxelHeight, voxelDepth);

        $.each(result["bricks"], function(key, value) {
            brickMaterial = new THREE.MeshBasicMaterial( { color : getRandomColor() } );    
            $.each(value, function(i, voxelKey) {
                var voxelLocation = result["voxels"][voxelKey];
                cube = new THREE.Mesh( voxelGeometry, brickMaterial );
                console.log(voxelLocation);
                cube.position.x = voxelLocation.x * voxelWidth + ( voxelWidth / 2 );
                cube.position.y = voxelLocation.y * voxelHeight + ( voxelHeight / 2 );
                cube.position.z = voxelLocation.z * voxelDepth + (voxelDepth / 2);
                scene.add(cube);
            });
            
            

        });
    });
}

function toHex(str) {
    var result = '';
    for (var i=0; i<str.length; i++) {
      result += str.charCodeAt(i).toString(16);
    }
    return result;
}


function getRandomColor() {
    var letters = '0123456789ABCDEF'.split('');
    var color = '#';
    for (var i = 0; i < 6; i++ ) {
        color += letters[Math.floor(Math.random() * 16)];
    }
    return color;
}

function init() {
    container = document.createElement( 'div' );
    document.body.appendChild( container );

    camera = new THREE.PerspectiveCamera( 45, window.innerWidth / window.innerHeight, 1, 50000 );
    camera.position.x = 500;
    camera.position.z = 500;
    camera.position.y = 500;

    controls = new THREE.OrbitControls( camera );

    window.scene = new THREE.Scene();
    addGrid(window.scene);
    addVoxels(window.scene);

    var ambient = new THREE.AmbientLight( 0x101030 );
    window.scene.add( ambient );

    var directionalLight = new THREE.DirectionalLight( 0xffffff );
    directionalLight.position.set( 500, 0, 500 );
    window.scene.add( directionalLight );

    renderer = new THREE.WebGLRenderer();
    renderer.setClearColor( 0xAAAAAA );
    renderer.setPixelRatio( window.devicePixelRatio );
    renderer.setSize( window.innerWidth, window.innerHeight);
    container.appendChild( renderer.domElement );

    document.addEventListener( 'mousemove', onDocumentMouseMove, false );

    window.addEventListener( 'resize', onWindowResize, false );
}

function onWindowResize() {
    windowHalfX = window.innerWidth / 2;
    windowHalfY = window.innerHeight / 2;

    camera.aspect = window.innerWidth / window.innerHeight;
    camera.updateProjectionMatrix();

    renderer.setSize( window.innerWidth, window.innerHeight );
}


function onDocumentMouseMove(event) {
    mouseX = ( event.clientX - windowHalfX ) / 2;
    mouseY = ( event.clientY - windowHalfY ) / 2;
}

function animate() {
    requestAnimationFrame(animate);
    render();
}

function render() {
    renderer.render( window.scene, camera );
}
