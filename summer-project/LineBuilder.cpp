#include "LineBuilder.h"
#include "Line.h"

// c'tor
LineBuilder::LineBuilder(std::valarray<double> res, std::valarray<double> a) {
	resolution.resize(2);
	std::copy(std::begin(res), std::end(res), std::begin(resolution));
	angle = a * Constants::DEGREE_TO_RADIANS_RATIO; // degrees -> radians
}

Line LineBuilder::getLine(Vec3d pos, std::valarray<double> pixel) const {
	// casting int array into double array (to be used later)
	std::valarray<double> pixeld = std::valarray<double>(2);
	std::copy(std::begin(pixel), std::end(pixel), std::begin(pixeld));
	
	pixeld = pixeld - (resolution / 2.0); //normalizing pixel pos relative to center
	pixeld[1] = -pixeld[1]; // coordinates (>x, ^y, .z)
	std::valarray<double> temp = (pixeld * tan(angle / 2.0)) / (resolution / 2.0); // calculating x,y length relative to z (=1)
	Vec3d v = { temp[0], temp[1], 1 }; //declaring the 3d line vector based on the calculation above
	
	return Line(pos, v);

}




/*
def gen_lines(inv_camera_matrix, image_pixel, init_camera_rot, init_camera_pos, rotation, translation):
	"""
	:param inv_camera_matrix: inverse camera matrix
	:param image_pixel: a 2D pixel array
	:param init_camera_rot: initial camera rotation, for first image should be np.eye(3)
	:param init_camera_pos: initial camera position, should be (0,0,0)
	:param rotation: rotation between images
	:param translation: translation between images
	:return: resulting line of the given 2D pixels
	"""
	image_lines = [inv_camera_matrix.dot(np.array([x, y, 1])) for x, y in image_pixel]
	image_lines = [line / np.linalg.norm(line, 2) for line in image_lines]
	camera_poses = [init_camera_pos]

	new_lines = [[]]
	for image_line in lines[0]:
		new_lines[0].append(Line(init_camera_pos, image_line))
	rotation_all = init_camera_rot
	for image_lines in lines[1:]:
		last_camera_pos = camera_poses[-1]
		current_camera_pos = rotation.dot(last_camera_pos) + translation
		camera_poses.append(current_camera_pos)
		rotation_all = rotation.dot(rotation_all)
		current_directions = [rotation_all.dot(line) for line in image_lines]
		new_lines.append([])
		for direction in current_directions:
			new_lines[-1].append(Line(current_camera_pos, direction))
	return np.array(new_lines).transpose()


*/