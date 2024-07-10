#
# Copyright(C) 2023, RT Coorp.
from setuptools import find_packages, setup
from glob import glob
import os

package_name = 'triorb_ros2'

setup(
    name=package_name,
    version='0.0.0',
    packages=find_packages(exclude=['test']),
    data_files=[
        ('share/ament_index/resource_index/packages',
            ['resource/' + package_name]),
        ('share/' + package_name, ['package.xml']),
        (os.path.join('share', package_name, 'launch'), glob(os.path.join('launch', '*launch.[pxy][yma]*'))),
        (os.path.join('share', package_name, 'config'), glob(os.path.join('config', '*.[yma]*'))),
    ],
    install_requires=['setuptools'],
    zip_safe=True,
    maintainer='hara',
    maintainer_email='hara@rt-net.jp',
    description='Control node for TriOrb',
    license='Apache License version 2.0',
    tests_require=['pytest'],
    entry_points={
        'console_scripts': [
            'triorb = triorb_ros2.triorb:main'
        ],
    },
)
