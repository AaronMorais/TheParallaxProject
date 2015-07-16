var camera, renderer, container;

var controls, raycaster, mouse;

// Raycasting interface globals
var hiddenBricks = [], visibleBricks = [], intersectedBrick;
var mouse = {};
var brickKeyToMesh = {};


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


function getDistanceSquaredFromOrigin(pointObj) {
    return pointObj.x * pointObj.x + pointObj.y * pointObj.y + pointObj.z * pointObj.z;
}

function getClosestPointToOrigin(voxelMap, voxelKeys) {
    var closestPoint = voxelMap[voxelKeys[0]];
    var closestPointDistance = getDistanceSquaredFromOrigin(closestPoint);

    $.each(voxelKeys, function(i, voxel) {
        var point = voxelMap[voxel];
        var currentDistance = getDistanceSquaredFromOrigin(point);        
        if( currentDistance < closestPointDistance ) {
            closestPoint = point;
            closestPointDistance = currentDistance;
        }
    });

    return closestPoint;
}

function getOneByOne(location, material) {
    var voxelWidth = 50;
    var voxelHeight = (50 * 3.2) / 15.9;
    var voxelDepth = 50;

    var cylinderHeight = 10.89;
    var cylinderRadius = 15.38;
    var renderLocation = {};
    renderLocation.x = location.x * voxelWidth + ( voxelWidth / 2 );
    renderLocation.y = location.y * voxelHeight + ( voxelHeight / 2 );
    renderLocation.z = location.z * voxelDepth + (voxelDepth / 2);

    var voxelGeometry = new THREE.BoxGeometry(voxelWidth, 
                                              voxelHeight, 
                                              voxelDepth);

    var voxelCylinder = new THREE.CylinderGeometry(cylinderRadius, 
                                                   cylinderRadius, 
                                                   cylinderHeight);
    // Moving the cylinder up above the voxel
    $.each(voxelCylinder.vertices, function(i, vertex) {
        vertex.y += (voxelHeight/2);
    });

    voxelGeometry.merge(voxelCylinder);

    var oneByOneMesh = new THREE.Mesh( voxelGeometry, material );
    oneByOneMesh.position.x = renderLocation.x;
    oneByOneMesh.position.y = renderLocation.y; 
    oneByOneMesh.position.z = renderLocation.z;

    return oneByOneMesh;
}


function addVoxels(scene) {
    $.getJSON("lego.json" , function(result) {
        var colors = {};
    
        $.each(result["bricks"], function(key, value) {
            brickMaterial = new THREE.MeshLambertMaterial( { color : getRandomColor() } );    
            $.each(value, function(i, voxelKey) {
                var voxelLocation = result["voxels"][voxelKey];
                cube = getOneByOne( voxelLocation, brickMaterial );
                cube.brickKey = key;
                if( !brickKeyToMesh[key] ) {
                    brickKeyToMesh[key] = [];
                }
                brickKeyToMesh[key].push(cube);

                scene.add(cube);
                visibleBricks.push(cube);
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

    // Controls for rotating the scene
    controls = new THREE.OrbitControls( camera );
    raycaster = new THREE.Raycaster();

    // Geometry scenes
    window.scene = new THREE.Scene();
    addGrid(window.scene);
    addVoxels(window.scene);

    // lights
    var ambient = new THREE.AmbientLight( 0x101030 );
    window.scene.add( ambient );

    var directionalLight = new THREE.DirectionalLight( 0xffffff );
    directionalLight.position.set( 500, 0, 500 );
    window.scene.add( directionalLight );

    var directionalLight2 = new THREE.DirectionalLight( 0xffffff );
    directionalLight2.position.set( 0, -1000, 0 );
    window.scene.add( directionalLight2 );

    var spotLight = new THREE.SpotLight( 0xffffff, 1.0 );
    spotLight.position.set( 500, 500, 2000 );
    spotLight.castShadow = true;

    spotLight.shadowCameraNear = 200;
    spotLight.shadowCameraFar = camera.far;
    spotLight.shadowCameraFov = 50;

    spotLight.shadowBias = -0.00022;
    spotLight.shadowDarkness = 0.5;

    spotLight.shadowMapWidth = 2048;
    spotLight.shadowMapHeight = 2048;

    scene.add( spotLight );

    // renderer
    renderer = new THREE.WebGLRenderer();
    renderer.setClearColor( 0xAAAAAA );
    renderer.setPixelRatio( window.devicePixelRatio );
    renderer.setSize( window.innerWidth, window.innerHeight);
    container.appendChild( renderer.domElement );

    document.addEventListener( 'mousemove', onDocumentMouseMove, false );
    document.addEventListener( 'keypress', onKeyPress, false);

    window.addEventListener( 'resize', onWindowResize, false );
}

function onKeyPress(event) {
    if( event.keyCode == 104 && intersectedBrick != undefined ) { // h keycode
        var selectedVoxelList = brickKeyToMesh[intersectedBrick.brickKey];
        hiddenBricks.push(selectedVoxelList);
        $.each(selectedVoxelList, function(i, voxel) {
            var index = visibleBricks.indexOf(voxel);
            visibleBricks.splice(index, 1);
            scene.remove(voxel);
        });
    } else if( event.keyCode == 117 ) { // u keycode 
        if( hiddenBricks.length > 0 ) {
            var revisibleBricks = hiddenBricks[hiddenBricks.length-1];
            $.each(revisibleBricks, function(i, revisibleBrick) {
                visibleBricks.push(revisibleBrick);
                scene.add(revisibleBrick);
            });
            hiddenBricks.pop();
        }
    }
}


function onWindowResize() {
    windowHalfX = window.innerWidth / 2;
    windowHalfY = window.innerHeight / 2;

    camera.aspect = window.innerWidth / window.innerHeight;
    camera.updateProjectionMatrix();

    renderer.setSize( window.innerWidth, window.innerHeight );
}


function onDocumentMouseMove(event) {
    event.preventDefault();
    mouse.x = ( event.clientX / window.innerWidth ) * 2 - 1;
    mouse.y = - ( event.clientY / window.innerHeight ) * 2 + 1;
    raycaster.setFromCamera( mouse, camera );

    var intersects = raycaster.intersectObjects(visibleBricks);
    if( intersects.length > 0 ) {
        if( intersectedBrick == undefined || 
            intersectedBrick.brickKey != intersects[0].object.brickKey ) {
            if( intersectedBrick ) {
                intersectedBrick.material.color.setHex(intersectedBrick.originalHex);
            }
            intersectedBrick = intersects[0].object;
            intersectedBrick.originalHex = intersectedBrick.material.color.getHex();
            intersectedBrick.material.color.setHex(0xffffff);
        } 
    } else {
        if( intersectedBrick )
            intersectedBrick.material.color.setHex(intersectedBrick.originalHex);
        intersectedBrick = undefined;
    }
}

function animate() {
    requestAnimationFrame(animate);
    render();
}

function render() {
    renderer.render( window.scene, camera );
}
