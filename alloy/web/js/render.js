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
        $.each(result["bricks"], function(key, value) {
            colour = colourNameToHex(result["colours"][key].split("$")[0]);
            brickMaterial = new THREE.MeshLambertMaterial( { color : colour } );
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

function colourNameToHex(colour) {
    var colours = {"aliceblue":"#f0f8ff","antiquewhite":"#faebd7","aqua":"#00ffff","aquamarine":"#7fffd4","azure":"#f0ffff",
    "beige":"#f5f5dc","bisque":"#ffe4c4","black":"#000000","blanchedalmond":"#ffebcd","blue":"#0000ff","blueviolet":"#8a2be2","brown":"#a52a2a","burlywood":"#deb887",
    "cadetblue":"#5f9ea0","chartreuse":"#7fff00","chocolate":"#d2691e","coral":"#ff7f50","cornflowerblue":"#6495ed","cornsilk":"#fff8dc","crimson":"#dc143c","cyan":"#00ffff",
    "darkblue":"#00008b","darkcyan":"#008b8b","darkgoldenrod":"#b8860b","darkgray":"#a9a9a9","darkgreen":"#006400","darkkhaki":"#bdb76b","darkmagenta":"#8b008b","darkolivegreen":"#556b2f",
    "darkorange":"#ff8c00","darkorchid":"#9932cc","darkred":"#8b0000","darksalmon":"#e9967a","darkseagreen":"#8fbc8f","darkslateblue":"#483d8b","darkslategray":"#2f4f4f","darkturquoise":"#00ced1",
    "darkviolet":"#9400d3","deeppink":"#ff1493","deepskyblue":"#00bfff","dimgray":"#696969","dodgerblue":"#1e90ff",
    "firebrick":"#b22222","floralwhite":"#fffaf0","forestgreen":"#228b22","fuchsia":"#ff00ff",
    "gainsboro":"#dcdcdc","ghostwhite":"#f8f8ff","gold":"#ffd700","goldenrod":"#daa520","gray":"#808080","green":"#008000","greenyellow":"#adff2f",
    "honeydew":"#f0fff0","hotpink":"#ff69b4",
    "indianred ":"#cd5c5c","indigo":"#4b0082","ivory":"#fffff0","khaki":"#f0e68c",
    "lavender":"#e6e6fa","lavenderblush":"#fff0f5","lawngreen":"#7cfc00","lemonchiffon":"#fffacd","lightblue":"#add8e6","lightcoral":"#f08080","lightcyan":"#e0ffff","lightgoldenrodyellow":"#fafad2",
    "lightgrey":"#d3d3d3","lightgreen":"#90ee90","lightpink":"#ffb6c1","lightsalmon":"#ffa07a","lightseagreen":"#20b2aa","lightskyblue":"#87cefa","lightslategray":"#778899","lightsteelblue":"#b0c4de",
    "lightyellow":"#ffffe0","lime":"#00ff00","limegreen":"#32cd32","linen":"#faf0e6",
    "magenta":"#ff00ff","maroon":"#800000","mediumaquamarine":"#66cdaa","mediumblue":"#0000cd","mediumorchid":"#ba55d3","mediumpurple":"#9370d8","mediumseagreen":"#3cb371","mediumslateblue":"#7b68ee",
    "mediumspringgreen":"#00fa9a","mediumturquoise":"#48d1cc","mediumvioletred":"#c71585","midnightblue":"#191970","mintcream":"#f5fffa","mistyrose":"#ffe4e1","moccasin":"#ffe4b5",
    "navajowhite":"#ffdead","navy":"#000080",
    "oldlace":"#fdf5e6","olive":"#808000","olivedrab":"#6b8e23","orange":"#ffa500","orangered":"#ff4500","orchid":"#da70d6",
    "palegoldenrod":"#eee8aa","palegreen":"#98fb98","paleturquoise":"#afeeee","palevioletred":"#d87093","papayawhip":"#ffefd5","peachpuff":"#ffdab9","peru":"#cd853f","pink":"#ffc0cb","plum":"#dda0dd","powderblue":"#b0e0e6","purple":"#800080",
    "red":"#ff0000","rosybrown":"#bc8f8f","royalblue":"#4169e1",
    "saddlebrown":"#8b4513","salmon":"#fa8072","sandybrown":"#f4a460","seagreen":"#2e8b57","seashell":"#fff5ee","sienna":"#a0522d","silver":"#c0c0c0","skyblue":"#87ceeb","slateblue":"#6a5acd","slategray":"#708090","snow":"#fffafa","springgreen":"#00ff7f","steelblue":"#4682b4",
    "tan":"#d2b48c","teal":"#008080","thistle":"#d8bfd8","tomato":"#ff6347","turquoise":"#40e0d0",
    "violet":"#ee82ee",
    "wheat":"#f5deb3","white":"#ffffff","whitesmoke":"#f5f5f5",
    "yellow":"#ffff00","yellowgreen":"#9acd32"};

    if (typeof colours[colour.toLowerCase()] != 'undefined')
        return colours[colour.toLowerCase()];

    console.log("No colour found for: ", colour);
    return getRandomColor();
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
