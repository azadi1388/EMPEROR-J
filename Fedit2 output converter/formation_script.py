dir = '/home/NAME/offense-formation_NEW.conf'
# You have to change dir based on directory your formation is saved at.
f = open(dir)
lines = f.readlines()

started = False
finished = False

print('area func: \n')

defenition = '''const WorldModel &wm = agent->world();
Vector2D ball_pos = wm.ball().pos();
std::vector <Vector2D> poses;'''
print(defenition)

for line in lines:
    if started:
        line = line.split()
        if line[0] == 'Ball':
            x = line[1]
            y = line[2]
            print(f'poses.push_back(Vector2D({x}, {y}))')
    else:
        line = line.split()
        if line[0] == 'Begin' and line[1] != 'Roles':
            started = True

search = '''
Vector2D nearest = poses[0];
int count = 0;
for(int i=1; i<poses.size(); i++)
{
    if(ball_pos.dist(poses[i]) < ball_pos.dist(nearest))
    {
        nearest = poses[i];
        count = i;
    }
}
return count;
'''
print(search)

print('hybrid_func:\n')

defenition = '''int count = area(agent);
std::vector <Vector2D> poses(12);
'''
print(defenition)

started = False
finished = False

for line in lines:
    if started:
        line = line.split()

        if line[0] != 'Ball' and line[0] != '-----' and line[0] != 'End':
            unum = line[0]
            x = line[1]
            y = line[2]
            print(f'poses.at({unum}) = Vector2D({x}, {y})')
            if unum == '11':
                print('}')
        
        if line[0] == '-----':
            count = line[1]
            print(f'\n\nif (count == {count})','{\n', sep='')

    else:
        line = line.split()
        if line[0] == 'Begin' and line[1] != 'Roles':
            started = True
        
print('return poses.at(agent->world().self().unum());')
